#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <ClientState.hpp>
#include <HTTPRequest.hpp>
#include <string>

class HTTPResponse
{
  private:
	int _status_code;
	ssize_t _bytes_sent;
	std::string _response;

  public:
	HTTPResponse(const HTTPRequest &request);
	~HTTPResponse();

	HTTPResponse();
	HTTPResponse(const HTTPResponse &other) = delete;
	HTTPResponse &operator=(const HTTPResponse &rhs) = delete;

	ClientState send(int client_fd);
};

#endif
