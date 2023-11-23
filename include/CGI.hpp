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
	// std::string	_body;

  public:
	CGI();
	CGI(const CGI &src) = delete;
	CGI &operator=(const CGI &rhs) = delete;
	~CGI();

	// execute is probably redundant
	void	start_CGI(const char *executable, char **env, size_t body_length);
	// void	execute(const char *executable);

	ClientState	receive(int pipe_fd, std::string body);
	// ClientState	send(int fd, HTTPMethod methodType, std::string requestBody);

	std::string	body;
	bool		body_has_been_sent;
	int			pipe_fd[2];
};

#endif
