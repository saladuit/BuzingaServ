#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "CGI.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class Client
{
  public:
	Client() = delete;
	Client(int fd);
	Client(const Client &other) = delete;
	const Client &operator=(const Client &other) = delete;
	~Client();

	void handle_connection();

  private:
	HTTPRequest _request;
	HTTPResponse _response;
	CGI _cgi;
	const int _fd;
};

#endif
