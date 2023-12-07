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

CGI::CGI() : _bodyBytesWritten(0) {}

CGI::~CGI() {}

const std::string &CGI::getExecutable(void) const {
	return (_executable);
}

void	CGI::setExecutable(std::string executable) {
	_executable = executable;
}

const pid_t &CGI::getPid(void) const {
	return (_pid);
}

ClientState CGI::send(Client &client ,std::string body, size_t bodyLength)
{
	(void)bodyLength;
	Logger	&logger = Logger::getInstance();
	ssize_t	bytesWritten = 0;

	logger.log(INFO, "GCI::send is called");
	logger.log(DEBUG, "body: " + body);
	if (!client.cgiBodyIsSent)
		bytesWritten = write(client.getServerToCgiFd()[WRITE_END], body.c_str(), BUFFER_SIZE);
	if (bytesWritten == SYSTEM_ERROR)
		throw SystemException("write to Python");
	logger.log(DEBUG, "bytesWritten: %", bytesWritten);
	logger.log(DEBUG, "_bodyBytesWritten before adding bytesWritten: %", _bodyBytesWritten);
	_bodyBytesWritten += bytesWritten;
	logger.log(DEBUG, "_bodyBytesWritten after adding bytesWritten: %", _bodyBytesWritten);
	logger.log(DEBUG, "bodyLength: %", bodyLength);
	if (_bodyBytesWritten >= bodyLength) 
	{
		client.cgiBodyIsSent = true;
		logger.log(DEBUG, "ServerToCgiFd()[WRITE_END] is now being closed");
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
	logger.log(DEBUG, "sizeof(buffer): %", sizeof(buffer));
	if (bytesRead < (ssize_t) sizeof(buffer)) 
	{
		client.cgiHasBeenRead = true;
		logger.log(DEBUG, "body in GCI::receive:\n" + body);
		close(client.getCgiToServerFd()[READ_END]);
		// int status;
		// waitpid(_pid, &status, 0);
		// if (WEXITSTATUS(status) == -1)
		// 	return (ClientState::Error);
		return (ClientState::Sending);
	}
	return (ClientState::CGI_Read);
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

	logger.log(DEBUG, "bodyLength: %", bodyLength);
	logger.log(DEBUG, "cgiBodyIsSent: %", client.cgiBodyIsSent);
	
	if (dup2(client.getCgiToServerFd()[READ_END], STDIN_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");
	std::shared_ptr<int> pipe = std::make_shared<int>(client.getCgiToServerFd()[READ_END]);
	active_pipes.emplace(client.getCgiToServerFd()[READ_END], pipe);
	poll.addPollFD(client.getCgiToServerFd()[READ_END], POLLIN);
	logger.log(DEBUG, "CGI::start after closing WRITE_END and start reading");

	if (bodyLength != 0 && !client.cgiBodyIsSent)
	{
		logger.log(DEBUG, "ClientState::CGI_Write is now being called");
		// if (dup2(client.getServerToCgiFd()[WRITE_END], STDOUT_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");
		std::shared_ptr<int> pipe = std::make_shared<int>(client.getServerToCgiFd()[WRITE_END]);
		active_pipes.emplace(client.getServerToCgiFd()[WRITE_END], pipe);
		poll.addPollFD(client.getServerToCgiFd()[WRITE_END], POLLOUT);
		// poll.setEvents(client.getServerToCgiFd()[WRITE_END], POLLOUT);
		return (ClientState::CGI_Write);
	}
	if (close(client.getServerToCgiFd()[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close");
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