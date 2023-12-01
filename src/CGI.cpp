#include "Client.hpp"
#include "Poll.hpp"
#include "CGI.hpp"
#include "Logger.hpp"
#include "SystemException.hpp"

#include <sys/wait.h>
#include <cassert>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>

// TO DO
// 	- get execute functional
// 	- make sure that the test.py is closing the STDIN if that is not already been done
// 	- insert in handleConnection CGI_WRITE and CGI_READ
//	- create some kind of a CGI_LOAD that serves as replacement for the file manager
//	- create python program
// 	* check out the wiki and github that Sander has sent

CGI::CGI() : _bodyIsSent(false), _bodyBytesWritten(0) {}

CGI::~CGI() {}

const std::string &CGI::getExecutable(void) const {
	return (_executable);
}

const pid_t &CGI::getPid(void) const {
	return (_pid);
}

ClientState CGI::send(Client &client ,std::string body, size_t bodyLength)
{
	Logger	&logger = Logger::getInstance();
	ssize_t	bytesWritten = 0;

	logger.log(INFO, "GCI::send is called");
	if (!_bodyIsSent) 
		bytesWritten = write(client.getServerToCgiFd()[WRITE_END], body.c_str(), BUFFER_SIZE);
	if (bytesWritten == SYSTEM_ERROR)
		throw SystemException("write to Python");
	_bodyBytesWritten += bytesWritten;
	if (_bodyBytesWritten == bodyLength) 
	{
		close(client.getServerToCgiFd()[WRITE_END]);
		return (ClientState::CGI_Read);
	}
	logger.log(INFO, "CGI body: " + std::to_string(bytesWritten));
	return (ClientState::CGI_Write);
}

ClientState	CGI::receive(Client &client)
{
	Logger	&logger = Logger::getInstance();
	ssize_t	bytesRead = 0;
	char 	buffer[1024];
	
	bzero(buffer, sizeof(buffer));
	logger.log(INFO, "CGI::receive is called");
	bytesRead = read(client.getCgiToServerFd()[READ_END], buffer, sizeof(buffer));

	if (bytesRead == SYSTEM_ERROR) throw SystemException("Read");
	logger.log(DEBUG, "Bytes read: " + std::to_string(bytesRead));
	logger.log(DEBUG, "buffer:\n" + std::string(buffer));
	body += buffer;
	// if (bytesRead >= 1024)
	// 	return (ClientState::CGI_Read);
	client.cgiHasBeenRead = true;
	// if (bytesRead == SYSTEM_ERROR)
	// 	return (ClientState::Error);
	logger.log(DEBUG, "body in GCI::receive:\n" + body);
	close(client.getCgiToServerFd()[READ_END]);
	// _poll.removeFD(poll_fd.fd); // optional

	// int status;
	// waitpid(_pid, &status, 0);
	// if (WEXITSTATUS(status) == -1)
	// 	return (ClientState::Error);
// }
	return (ClientState::Sending);
}

bool CGI::fileExists(const std::string& filePath) {
    return (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath));
}

bool CGI::isExecutable(const std::string& filePath) 
{
    std::filesystem::file_status fileStat = std::filesystem::status(filePath);

    return std::filesystem::is_regular_file(fileStat) && (fileStat.permissions() &
	std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void	CGI::execute(std::string executable, char **env)
{
	(void)env;
	Logger &logger = Logger::getInstance();
	std::string bin = "python3";

	
	logger.log(ERROR, "CGI::execute is called");
	logger.log(ERROR, "Executable: %", executable);
	// if (!fileExists(std::string(executable))) {
	// 	logger.log(ERROR, "Path to external program does not exist");
	// 	_exit(127);
	// }
	// if (!isExecutable(std::string(executable))) {
	// 	logger.log(ERROR, "External program is not executable");
	// 	_exit(127);
	// }

	std::string	executableWithPath = "data/www" + executable;
	logger.log(ERROR, "executableWithPath: " + executableWithPath);

	// const char *const argv[] = {bin.c_str(), executable.c_str(), NULL};
	const char *const argv[] = {bin.c_str(), executableWithPath.c_str(), NULL};
	// const char *path = "/data/www";
	const char *path = "/usr/bin/python3";
	// execve(path, (char *const *)argv, env);
	if (execve(path, (char *const *)argv, NULL) == SYSTEM_ERROR) throw SystemException("Execve");
}

ClientState CGI::start(Poll &poll, Client &client, size_t bodyLength, 
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes)
{
	(void)client;
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "CGI::start called");
	logger.log(DEBUG, "Executable: %", _executable);


	if (pipe(client.getServerToCgiFd()) == SYSTEM_ERROR) throw SystemException("Pipe");
	if (pipe(client.getCgiToServerFd()) == SYSTEM_ERROR) throw SystemException("Pipe");

	// if (fcntl(client.getServerToCgiFd()[READ_END], F_SETFL, O_NONBLOCK) == SYSTEM_ERROR) { perror("fcntl"); throw SystemException("fcntl");}
	// if (fcntl(client.getServerToCgiFd()[WRITE_END], F_SETFL, O_NONBLOCK) == SYSTEM_ERROR) { perror("fcntl"); throw SystemException("fcntl");}
	// if (fcntl(client.getCgiToServerFd()[READ_END], F_SETFL, O_NONBLOCK) == SYSTEM_ERROR) { perror("fcntl"); throw SystemException("fcntl");}
	// if (fcntl(client.getCgiToServerFd()[WRITE_END], F_SETFL, O_NONBLOCK) == SYSTEM_ERROR) { perror("fcntl"); throw SystemException("fcntl");}

	_pid = fork();
	if (_pid == SYSTEM_ERROR) throw SystemException("Fork");
	if (_pid == 0)
	{
		logger.log(ERROR, "_pid == 0");
		if (close(client.getServerToCgiFd()[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close"); 
		if (close(client.getCgiToServerFd()[READ_END]) == SYSTEM_ERROR) throw SystemException("close");
		if (dup2(client.getServerToCgiFd()[READ_END], STDIN_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");
		if (close(client.getServerToCgiFd()[READ_END]) == SYSTEM_ERROR) throw SystemException("close");
		if (dup2(client.getCgiToServerFd()[WRITE_END], STDOUT_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");
		if (close(client.getCgiToServerFd()[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close");
		
		execute(_executable, _env);
	}
	logger.log(DEBUG, "CGI::start after else if (_pid == 0)");
	if (close(client.getServerToCgiFd()[READ_END]) == SYSTEM_ERROR) throw SystemException("close"); 
	if (close(client.getCgiToServerFd()[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close");
	logger.log(DEBUG, "CGI::start after closing");

	if (bodyLength != 0)
		return (ClientState::CGI_Write);
	logger.log(DEBUG, "GCI::start after writing");
	if (close(client.getServerToCgiFd()[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close");
	if (dup2(client.getCgiToServerFd()[READ_END], STDIN_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");

	std::shared_ptr<int> pipe = std::make_shared<int>(client.getCgiToServerFd()[READ_END]);
	active_pipes.emplace(client.getCgiToServerFd()[READ_END], pipe);
	poll.addPollFD(client.getCgiToServerFd()[READ_END], POLLIN);
	logger.log(DEBUG, "CGI::start after closing WRITE_END and start reading");

	// if (close(client.getCgiToServerFd()[READ_END]) == SYSTEM_ERROR) throw SystemException("close");
	// int	status;
	// waitpid()
	// return (receive());
	return (ClientState::CGI_Read);
	// return (ClientState::Done);
}

// !! need to free _env and it's arguments somewhere !!
ClientState	CGI::parseURIForCGI(std::string requestTarget)
{
	Logger 		&logger = Logger::getInstance();
	logger.log(DEBUG, "parseURIForCGI is called");
	logger.log(DEBUG, "requestTarget: %", requestTarget);
	// return (ClientState::Loading);

	std::string	filenameExtension = ".py"; // or something like: "data/www/python/test.py" to specify it better. OR give it as input. Discuss with the team!
	size_t		lengthFilenameExtension = std::strlen(filenameExtension.c_str());
    size_t		filenameExtensionPos = requestTarget.find(filenameExtension);
	logger.log(DEBUG, "Length of filenameExtension: %", lengthFilenameExtension);
	if (filenameExtensionPos == std::string::npos)
		return (ClientState::Error);
		// return (ClientState::Done);

    _executable = requestTarget.substr(0, filenameExtensionPos + lengthFilenameExtension);
	bool		skip = false;
	size_t		env_num = 1;
	size_t		i = 0;

	logger.log(DEBUG, "Executable is: " + _executable);
	if (filenameExtensionPos + lengthFilenameExtension >= std::strlen(requestTarget.c_str()) - 1) {
		// logger.log(DEBUG, "filenameExtensionPos + lengthFilenameExtension >= std::strlen(requestTarget.c_str()) - 1");
		return (ClientState::CGI_Start);
		// return (ClientState::Done);
	}

	logger.log(DEBUG, "filenameExtensionPos + lengthfilenameExtension: %", filenameExtensionPos + lengthFilenameExtension);
	
	std::string	remaining = requestTarget.substr(filenameExtensionPos + lengthFilenameExtension, std::string::npos);
	size_t		questionMarkPos = remaining.find('?');
	if (remaining.at(0) == '/')
	{
		env_num++;
		if (questionMarkPos != std::string::npos)
			env_num++;
	}
	else if (remaining.at(0) == '?')
		env_num++;
	_env = new char *[env_num];
	if (remaining.at(0) == '/' && !skip)
	{
		std::string pathInfo = "PATH_INFO=" + remaining.substr(0, questionMarkPos);
		// vector string instead of new char shit
		_env[i] = new char[pathInfo.length() + 1];
		std::strcpy(_env[i], pathInfo.c_str());
		if (env_num == 2)
		{	_env[i + 1] = nullptr; skip = true;}
		i++;
	}
	if (!skip) {
		std::string queryString = "QUERY_STRING=" + remaining.substr(questionMarkPos, std::string::npos);
	_env[i] = new char[queryString.length() + 1];
	std::strcpy(_env[i], queryString.c_str());
	_env[i + 1] = nullptr; }
	
	for (size_t i = 0; _env[i]; i++) {
		logger.log(DEBUG, _env[i]);
	}
	logger.log(INFO, "Do we reach the end of parseURIForCGI");
	return (ClientState::CGI_Start);
	// return (ClientState::CGI_Start);
}