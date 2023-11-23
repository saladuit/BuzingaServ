#include <CGI.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>

#include <cassert>
#include <fcntl.h>
#include <unistd.h>


// TO DO
// 	- make in startCGI connection (pollFD) for out[READ_END] and optionally for in[WRITE_END] (in case of post request)
// 	- insert in handleConnection CGI_WRITE and CGI_READ
//	- create some kind of a CGI_LOAD that serves as replacement for the file manager
//	- create python program
// 	* check out the wiki and github that Sander has sent

CGI::CGI() : _bodyIsSent(false), _bodyBytesWritten(0) {}

CGI::~CGI() {}

ClientState CGI::send(std::string body, size_t bodyLength)
{
	ssize_t	writeSize = 0;

	if (!_bodyIsSent) 
		writeSize = write(_serverToPython[WRITE_END], &body, BUFFER_SIZE);
	if (writeSize == SYSTEM_ERROR)
		throw SystemException("write to Python");
	_bodyBytesWritten += writeSize;
	if (_bodyBytesWritten == bodyLength) 
	{
		close(_serverToPython[WRITE_END]);
		return (ClientState::CGI_Read);
	}
	return (ClientState::CGI_Write);
}

ClientState	CGI::receive(int fd, std::string body)
{
	Logger &logger = Logger::getInstance();
	char buffer[1024];
	int read_bytes;
	bzero(buffer, sizeof(buffer));
	read_bytes = read(fd, buffer, sizeof(buffer));
	logger.log(INFO, "Bytes read: " + std::to_string(read_bytes));
	
	body += buffer;
	// is it necesssary to check here whether the eof is reached on the fd?
	return (ClientState::Loading);
}

void	CGI::execute(const char *executable, char **env)
{
	Logger &logger = Logger::getInstance();
	std::string bin = "python3";
	
	const char *const argv[] = {bin.c_str(), executable, NULL};
	const char *path = "/usr/bin/python3";
	execve(path, (char *const *)argv, env);

	// if error throw exception?
	logger.log(ERROR, "execve error" + std::string(strerror(errno)));
	_exit(127);
}

ClientState CGI::start(const char *executable, char **env, size_t bodyLength)
{
	// int infile = open("in.txt", O_RDONLY, 0666);
	// int outfile = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "CGI::start called");
	return (ClientState::Loading);

	logger.log(INFO, "launching CGI");
	if (pipe(_serverToPython) == SYSTEM_ERROR)
		throw SystemException("Pipe");
	if (pipe(_pythonToServer) == SYSTEM_ERROR)
		throw SystemException("Pipe");

	_pid = fork();
	if (_pid == SYSTEM_ERROR)
		throw SystemException("Fork");
	else if (_pid == 0)
	{
		if (close(_serverToPython[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close"); 
		if (close(_pythonToServer[READ_END]) == SYSTEM_ERROR) throw SystemException("close");
		if (dup2(_serverToPython[READ_END], STDIN_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");
		if (close(_serverToPython[READ_END]) == SYSTEM_ERROR) throw SystemException("close");
		if (dup2(_pythonToServer[WRITE_END], STDOUT_FILENO) == SYSTEM_ERROR) throw SystemException("dup2");
		if (close(_pythonToServer[WRITE_END]) == SYSTEM_ERROR) throw SystemException("close");
		
		execute(executable, env);
	}
	assert(close(_serverToPython[READ_END]) && "close in[READ_END] error");
	assert(close(_pythonToServer[WRITE_END]) && "close out[WRITE_END] error");

	if (bodyLength != 0)
		return (ClientState::CGI_Write);
	if (close(_serverToPython[WRITE_END]) == SYSTEM_ERROR) 
		throw SystemException("close");
	return (ClientState::CGI_Read);


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
