#ifndef CGI_HPP
#define CGI_HPP

#include <ClientState.hpp>
#include <HTTPRequest.hpp>
#include <string>

#define READ_END 0
#define WRITE_END 1

// Common gateway interface

class CGI
{
private:
	pid_t	_pid;
	int		_serverToPython[2];
	int		_pythonToServer[2];
	bool	_bodyIsSent;
	size_t	_bodyBytesWritten;

  public:
	CGI();
	CGI(const CGI &src) = delete;
	CGI &operator=(const CGI &rhs) = delete;
	~CGI();

	// execute is probably redundant
	ClientState	start(const char *executable, char **env, size_t body_length);
	void	execute(const char *executable, char **env);

	ClientState	send(std::string body, size_t bodyLength);

	ClientState	receive(int pipe_fd, std::string body);
	// ClientState	send(int fd, HTTPMethod methodType, std::string requestBody);

	std::string	body;
	int			pipe_fd[2];
};

#endif
