#include <CGI.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>

#include <cassert>
#include <fcntl.h>
#include <unistd.h>


CGI::CGI()
{
}

CGI::~CGI()
{
}

// Every step in the CGI must be done through poll
// First step: separate on read, write and execve
// First read is already been done by the request parser,
// so after creating the pipe in the execute function return to poll and
// wait till it's says it's ready to write.
void	CGI::execute(const char *executable, bool get, std::string &body)
{
	Logger	&logger = Logger::getInstance();
	int		fd[2];
	pid_t	pid;
	
	// STEP 1
	if (pipe(fd) == SYSTEM_ERROR)
		throw SystemException("Pipe");
	// *return to poll with fd[write_end]

	// STEP 2
	logger.log(INFO, "launching CGI");
	if (get) {
		logger.log(INFO, "handling CGI GET Request");

		pid = fork();
		if (pid == SYSTEM_ERROR)
			throw SystemException("Fork");
		else if (pid == 0) {
			if (dup2(fd[WRITE_END], STDOUT_FILENO) == SYSTEM_ERROR)
			{
				logger.log(ERROR, "dup2 out[WRITE_END] error" +
									std::string(strerror(errno)));
				_exit(127);
			}
			std::string bin = "python3";
			const char *const argv[] = {bin.c_str(), executable, NULL};
			char *const envp[] = {NULL};
			const char *path = "/usr/bin/python3";
			execve(path, (char *const *)argv, envp);
			logger.log(ERROR, "execve error" + std::string(strerror(errno)));
			_exit(127);
		}
		// *return to poll with fd[read_end]

		// STEP 3
		char buffer[1024];
		int read_bytes;
		bzero(buffer, sizeof(buffer));
		read_bytes = read(fd[READ_END], buffer, sizeof(buffer));
		logger.log(INFO, "Bytes read: " + std::to_string(read_bytes));
		body += buffer;
		// logger.log(INFO, "Output: " + std::string(buffer));
		// *return to poll

		// FINAL STEP
		// response

	}
	else {
		// implementation for post request
		logger.log(INFO, "handling CGI POST Request");
	}
	close(fd[READ_END]);
	close(fd[WRITE_END]);
}

// void CGI::execute(const char *executable)
// {
	// int infile = open("in.txt", O_RDONLY, 0666);
	// int outfile = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	// Logger &logger = Logger::getInstance();
	// int in[2];
	// int out[2];
	// pid_t pid;

	// logger.log(INFO, "launching CGI");
	// if (pipe(in) == SYSTEM_ERROR)
	// 	throw SystemException("Pipe");
	// if (pipe(out) == SYSTEM_ERROR)
	// 	throw SystemException("Pipe");

	// pid = fork();
	// if (pid == SYSTEM_ERROR)
	// 	throw SystemException("Fork");
	// else if (pid == 0)
	// {
	// 	if (close(in[WRITE_END]) == SYSTEM_ERROR)
	// 	{
	// 		logger.log(ERROR, "close in[WRITE_END] error" +
	// 							  std::string(strerror(errno)));
	// 		_exit(127);
	// 	}
	// 	if (close(out[READ_END]) == SYSTEM_ERROR)
	// 	{
	// 		logger.log(ERROR, "close out[READ_END] error" +
	// 							  std::string(strerror(errno)));
	// 		_exit(127);
	// 	}
	// 	if (dup2(in[READ_END], STDIN_FILENO) == SYSTEM_ERROR)
	// 	{
	// 		logger.log(ERROR, "dup2 in[READ_END] error" +
	// 							  std::string(strerror(errno)));
	// 		_exit(127);
	// 	}
	// 	if (close(in[READ_END]) == SYSTEM_ERROR)
	// 	{
	// 		logger.log(ERROR, "close in[READ_END] error" +
	// 							  std::string(strerror(errno)));
	// 		_exit(127);
	// 	}
	// 	if (dup2(out[WRITE_END], STDOUT_FILENO) == SYSTEM_ERROR)
	// 	{
	// 		logger.log(ERROR, "dup2 out[WRITE_END] error" +
	// 							  std::string(strerror(errno)));
	// 		_exit(127);
	// 	}
	// 	if (close(out[WRITE_END]) == SYSTEM_ERROR)
	// 	{
	// 		logger.log(ERROR, "close out[WRITE_END] error" +
	// 							  std::string(strerror(errno)));
	// 		_exit(127);
	// 	}
		// std::string bin = "python3";
		//
		// std::string script = "./cgi-bin/print.py";
		// const char *const argv[] = {bin.c_str(), script.c_str(), NULL};
		//
		// const char *const argv[] = {bin.c_str(), executable, NULL};
		// char *const envp[] = {NULL};
		// const char *path = "/usr/bin/python3";
		// execve(path, (char *const *)argv, envp);
	// 	logger.log(ERROR, "execve error" + std::string(strerror(errno)));
	// 	_exit(127);
	// }
	// assert(close(in[READ_END]) && "close in[READ_END] error");
	// assert(close(out[WRITE_END]) && "close out[WRITE_END] error");

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
// }
