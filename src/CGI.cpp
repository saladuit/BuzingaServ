#include <CGI.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>

#include <cassert>
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

ClientState CGI::send(std::string body, size_t bodyLength)
{
	Logger	&logger = Logger::getInstance();
	ssize_t	bytesWritten = 0;

	logger.log(INFO, "GCI::send is called");
	if (!_bodyIsSent) 
		bytesWritten = write(_serverToExternalProgram[WRITE_END], &body, BUFFER_SIZE);
	if (bytesWritten == SYSTEM_ERROR)
		throw SystemException("write to Python");
	_bodyBytesWritten += bytesWritten;
	if (_bodyBytesWritten == bodyLength) 
	{
		close(_serverToExternalProgram[WRITE_END]);
		return (ClientState::CGI_Read);
	}
	logger.log(INFO, "CGI body: " + std::to_string(bytesWritten));
	return (ClientState::CGI_Write);
}

ClientState	CGI::receive(std::string body)
{
	Logger	&logger = Logger::getInstance();
	ssize_t	bytesRead = 0;
	
	logger.log(INFO, "GCI::receive is called");
	char 	buffer[1024];
	bzero(buffer, sizeof(buffer));
	bytesRead = read(_externalProgramToServer[READ_END], buffer, sizeof(buffer));
	logger.log(INFO, "Bytes read: " + std::to_string(bytesRead));
	body += buffer;
	if (bytesRead == 0)
		return (ClientState::CGI_Read);
	if (bytesRead == SYSTEM_ERROR)
		return (ClientState::Error);
	return (ClientState::Loading);
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

	const char *const argv[] = {bin.c_str(), executable.c_str(), NULL};
	const char *path = "/data/www";
	// const char *path = "/usr/bin/python3";
	execve(path, (char *const *)argv, env);

	// if error throw exception?
	logger.log(ERROR, "execve error" + std::string(strerror(errno)));
	_exit(127);
}

ClientState CGI::start(std::string executable, char **env, size_t bodyLength)
{
	// int infile = open("in.txt", O_RDONLY, 0666);
	// int outfile = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "CGI::start called");
	logger.log(DEBUG, "Executable: %", executable);

	// return (ClientState::Loading);

	if (pipe(_serverToExternalProgram) == SYSTEM_ERROR) throw SystemException("Pipe");
	if (pipe(_externalProgramToServer) == SYSTEM_ERROR) throw SystemException("Pipe");

	_pid = fork();
	if (_pid == SYSTEM_ERROR)
		throw SystemException("Fork");
	else if (_pid == 0)
	{
		if (close(_serverToExternalProgram[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close"); 
		if (close(_externalProgramToServer[READ_END]) == SYSTEM_ERROR) throw SystemException("close");
		if (dup2(_serverToExternalProgram[READ_END], STDIN_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");
		if (close(_serverToExternalProgram[READ_END]) == SYSTEM_ERROR) throw SystemException("close");
		if (dup2(_externalProgramToServer[WRITE_END], STDOUT_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");
		if (close(_externalProgramToServer[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close");
		
		execute(executable, env);
	}
	logger.log(DEBUG, "CGI::start after else if (_pid == 0)");
	if (close(_serverToExternalProgram[READ_END]) == SYSTEM_ERROR) throw SystemException("close"); 
	if (close(_externalProgramToServer[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close");
	//
	// if (close(_serverToExternalProgram[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close"); 
	// if (close(_externalProgramToServer[READ_END]) == SYSTEM_ERROR) throw SystemException("close");
	logger.log(DEBUG, "CGI::start after closing");
	// assert(close(_serverToExternalProgram[READ_END]));
	// assert(close(_externalProgramToServer[WRITE_END]));

	if (bodyLength != 0)
		return (ClientState::CGI_Write);
	if (close(_serverToExternalProgram[WRITE_END]) == SYSTEM_ERROR) 
		throw SystemException("close");
	return (ClientState::CGI_Read);
	// return (ClientState::Done);


	// are you alowed to execute and read in one function? NO
	// char buffer_out[1024];
	// char buffer_in[1024];
	// int read_bytes;
	// bzero(buffer_in, sizeof(buffer_in));
	// bzero(buffer_out, sizeof(buffer_out));

	// read_bytes = read(infile, buffer_in, sizeof(buffer_in));
	// logger.log(INFO, "buffer_in: " + std::string(buffer_in));
	// if (write(in[WRITE_END], buffer_in, read_bytes) == SYSTEM_ERROR)
	// 	logger.log(ERROR, "write error" + std::string(strerror(errno)));

	// assert(close(in[WRITE_END]) && "close in[WRITE_END] error");
	// if ((read_bytes = read(out[READ_END], buffer_out, sizeof(buffer_out))) ==
	// 	SYSTEM_ERROR)
	// 	logger.log(ERROR, "read error" + std::string(strerror(errno)));
	// logger.log(INFO, "buffer_out: " + std::string(buffer_out));
	// write(outfile, buffer_out, read_bytes);
	// close(out[READ_END]);

	// assert(close(infile) && "close infile error");
	// assert(close(outfile) && "close outfile error");
}
