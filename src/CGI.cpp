#include "CGI.hpp"
#include "Defines.hpp"
#include "Logger.hpp"
#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

CGI::CGI()
{
}

CGI::~CGI()
{
}

#define READ_END 0
#define WRITE_END 1

void CGI::execute()
{
	int infile = open("in.txt", O_RDONLY, 0666);
	int outfile = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "launcging CGI");
	int in[2];
	int out[2];
	pid_t pid;

	if (pipe(in) == G_ERROR)
		logger.log(ERROR, "pipe in error");
	if (pipe(out) == G_ERROR)
		logger.log(ERROR, "pipe out error");

	pid = fork();
	if (pid == G_ERROR)
		logger.log(ERROR, "fork error");
	else if (pid == 0)
	{
		if (close(in[WRITE_END]) == G_ERROR)
		{
			logger.log(ERROR, "close in[WRITE_END] error" +
								  std::string(strerror(errno)));
			return;
		}
		if (close(out[READ_END]) == G_ERROR)
		{
			logger.log(ERROR, "close out[READ_END] error" +
								  std::string(strerror(errno)));
			return;
		}
		if (dup2(in[READ_END], STDIN_FILENO) == G_ERROR)
		{
			logger.log(ERROR, "dup2 in[READ_END] error" +
								  std::string(strerror(errno)));
			return;
		}
		if (close(in[READ_END]) == G_ERROR)
		{
			logger.log(ERROR, "close in[READ_END] error" +
								  std::string(strerror(errno)));
			return;
		}
		if (dup2(out[WRITE_END], STDOUT_FILENO) == G_ERROR)
		{
			logger.log(ERROR, "dup2 out[WRITE_END] error" +
								  std::string(strerror(errno)));
			return;
		}
		if (close(out[WRITE_END]) == G_ERROR)
		{
			logger.log(ERROR, "close out[WRITE_END] error" +
								  std::string(strerror(errno)));
			return;
		}
		std::string bin = "python3";
		std::string script = "./cgi-bin/print.py";
		const char *const argv[] = {bin.c_str(), script.c_str(), NULL};
		// std::string
		char *const envp[] = {NULL};
		const char *path = "/usr/bin/python3";
		if (execve(path, (char *const *)argv, envp))
		{
			logger.log(ERROR, "execve error" + std::string(strerror(errno)));
		}
		_exit(127);
	}
	close(in[READ_END]);
	close(out[WRITE_END]);

	char buffer_out[1024];
	char buffer_in[1024];
	int read_bytes;
	bzero(buffer_in, sizeof(buffer_in));
	bzero(buffer_out, sizeof(buffer_out));

	read_bytes = read(infile, buffer_in, sizeof(buffer_in));
	logger.log(INFO, "buffer_in: " + std::string(buffer_in));
	if (write(in[WRITE_END], buffer_in, read_bytes) == G_ERROR)
		logger.log(ERROR, "write error" + std::string(strerror(errno)));

	close(in[WRITE_END]);
	if ((read_bytes = read(out[READ_END], buffer_out, sizeof(buffer_out))) ==
		G_ERROR)
		logger.log(ERROR, "read error" + std::string(strerror(errno)));
	logger.log(INFO, "buffer_out: " + std::string(buffer_out));
	write(outfile, buffer_out, read_bytes);
	close(out[READ_END]);

	close(infile);
	close(outfile);
}
