#ifndef CGI_HPP
#define CGI_HPP

#include <ClientState.hpp>
#include <string>

#define READ_END 0
#define WRITE_END 1

// Common gateway interface

class CGI
{
  private:
	std::string	_post_body;

  public:
	CGI();
	CGI(const CGI &src) = delete;
	CGI &operator=(const CGI &rhs) = delete;
	~CGI();

	// execute is probably redundant
	void execute(const char *executable, bool get, std::string &body);

	ClientState	receive(int pipe_fd, std::string body);
	ClientState	send(int fd);
};

#endif
