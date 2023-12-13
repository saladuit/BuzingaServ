#include "ClientException.hpp"
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

CGI::CGI() : _bodyBytesWritten(0) {
	_pathInfo = "";
	_subPathInfo = "";
	_queryString = "";
}

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

size_t		CGI::getBufferSize(size_t bodyLength)
{
	size_t	bufferSize = 0;

	if (BUFFER_SIZE > bodyLength)
		bufferSize = bodyLength;
	else if (BUFFER_SIZE > (bodyLength - _bodyBytesWritten))
		bufferSize = bodyLength - _bodyBytesWritten;
	else
		bufferSize = BUFFER_SIZE;
	return (bufferSize);
}

ClientState CGI::send(Client &client ,std::string body, size_t bodyLength)
{
	Logger	&logger = Logger::getInstance();
	ssize_t	bytesWritten = 0;
	
	logger.log(INFO, "GCI::send is called");
	logger.log(DEBUG, "body: " + body);

	if (!client.cgiBodyIsSent)
		bytesWritten = write(client.getServerToCgiFd()[WRITE_END], body.c_str(), getBufferSize(bodyLength));
	if (bytesWritten == SYSTEM_ERROR)
		throw ClientException(StatusCode::InternalServerError);
	logger.log(DEBUG, "bytesWritten: %", bytesWritten);
	_bodyBytesWritten += bytesWritten;
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
	int	status;
	waitpid(_pid, &status, 0);
	if (status == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
	bytesRead = read(client.getCgiToServerFd()[READ_END], buffer, sizeof(buffer));
	if (bytesRead == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
	logger.log(DEBUG, "Bytes read: " + std::to_string(bytesRead));
	logger.log(DEBUG, "buffer:\n" + std::string(buffer));
	body += buffer;
	logger.log(DEBUG, "sizeof(buffer): %", sizeof(buffer));
	if (bytesRead < (ssize_t) sizeof(buffer)) 
	{
		client.cgiHasBeenRead = true;
		logger.log(DEBUG, "body in GCI::receive:\n" + body);
		close(client.getCgiToServerFd()[READ_END]);
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

void	CGI::execute(std::string executable)
{
	const char  *env[] = {_pathInfo.c_str(), _queryString.c_str(), NULL};
	Logger &logger = Logger::getInstance();
	std::string bin = "python3";

	logger.log(ERROR, "CGI::execute is called");
	logger.log(ERROR, "Executable: %", executable);
	std::string	executableWithPath = "data/www" + executable;
	logger.log(ERROR, "executableWithPath: " + executableWithPath);
	const char *const argv[] = {bin.c_str(), executableWithPath.c_str(), _subPathInfo.c_str(), NULL};
	const char *path = "/usr/bin/python3";
	if (execve(path, (char *const *)argv, (char *const*) env) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
}

ClientState CGI::start(Poll &poll, Client &client, size_t bodyLength, 
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes)
{
	(void)client;
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "CGI::start called");
	logger.log(DEBUG, "Executable: %", _executable);
	if (pipe(client.getServerToCgiFd()) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
	if (pipe(client.getCgiToServerFd()) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
	_pid = fork();
	if (_pid == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
	if (_pid == 0)
	{
		if (close(client.getServerToCgiFd()[WRITE_END]) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
		if (close(client.getCgiToServerFd()[READ_END]) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
		if (dup2(client.getServerToCgiFd()[READ_END], STDIN_FILENO) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
		if (close(client.getServerToCgiFd()[READ_END]) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
		if (dup2(client.getCgiToServerFd()[WRITE_END], STDOUT_FILENO) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
		if (close(client.getCgiToServerFd()[WRITE_END]) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
		execute(_executable);
	}
	logger.log(DEBUG, "CGI::start after else if (_pid == 0)");
	if (close(client.getServerToCgiFd()[READ_END]) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
	if (close(client.getCgiToServerFd()[WRITE_END]) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
	logger.log(DEBUG, "CGI::start after closing");
	logger.log(DEBUG, "bodyLength: %", bodyLength);
	logger.log(DEBUG, "cgiBodyIsSent: %", client.cgiBodyIsSent);
	std::shared_ptr<int> pipe = std::make_shared<int>(client.getCgiToServerFd()[READ_END]);
	active_pipes.emplace(client.getCgiToServerFd()[READ_END], pipe);
	poll.addPollFD(client.getCgiToServerFd()[READ_END], POLLIN);
	logger.log(DEBUG, "CGI::start after closing WRITE_END and start reading");
	if (bodyLength != 0 && !client.cgiBodyIsSent)
	{
		logger.log(DEBUG, "ClientState::CGI_Write is now being called");
		std::shared_ptr<int> pipe = std::make_shared<int>(client.getServerToCgiFd()[WRITE_END]);
		active_pipes.emplace(client.getServerToCgiFd()[WRITE_END], pipe);
		poll.addPollFD(client.getServerToCgiFd()[WRITE_END], POLLOUT);
		return (ClientState::CGI_Write);
	}
	if (close(client.getServerToCgiFd()[WRITE_END]) == SYSTEM_ERROR) throw ClientException(StatusCode::InternalServerError);
	return (ClientState::CGI_Read);
}

ClientState	CGI::parseURIForCGI(std::string requestTarget)
{
	Logger 		&logger = Logger::getInstance();
	logger.log(DEBUG, "parseURIForCGI is called");
	logger.log(DEBUG, "requestTarget: %", requestTarget);
	std::string	filenameExtension = ".py";
	size_t		lengthFilenameExtension = std::strlen(filenameExtension.c_str());
    size_t		filenameExtensionPos = requestTarget.find(filenameExtension);
	
	logger.log(DEBUG, "Length of filenameExtension: %", lengthFilenameExtension);
	if (filenameExtensionPos == std::string::npos)
		return (ClientState::Error);
    _executable = requestTarget.substr(0, filenameExtensionPos + lengthFilenameExtension);
	bool		skip = false;
	logger.log(DEBUG, "Executable is: " + _executable);
	if (!fileExists("data/www" + _executable) || !isExecutable("data/www" + _executable))
		throw ClientException(StatusCode::NotFound);
	if (filenameExtensionPos + lengthFilenameExtension >= std::strlen(requestTarget.c_str()) - 1) {
		return (ClientState::CGI_Start);
	}
	std::string	remaining = requestTarget.substr(filenameExtensionPos + lengthFilenameExtension, std::string::npos);
	size_t		questionMarkPos = remaining.find('?');
	if (remaining.at(0) == '/' && !skip) {
		_subPathInfo = remaining.substr(0, questionMarkPos);
		_pathInfo = "PATH_INFO=" + _subPathInfo;
	}
	if (!skip)
		_queryString = "QUERY_STRING=" + remaining.substr(questionMarkPos, std::string::npos);
	logger.log(DEBUG, _pathInfo);
	logger.log(DEBUG, _queryString);
	return (ClientState::CGI_Start);
}