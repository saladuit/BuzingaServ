#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "CGI.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class Client
{
  public:
	Client();
	Client(const Client &other);
	const Client &operator=(const Client &other);
	~Client();

  private:
	HTTPRequest _request;
	HTTPResonse _response;
	CGI _cgi;
};

#endif
