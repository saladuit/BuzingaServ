#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <ClientState.hpp>
#include <HTTPRequest.hpp>
#include <fstream>
#include <string>

class HTTPResponse
{
  private:
	size_t _bytes_sent;
	std::string _response;

  public:
	HTTPResponse();
	HTTPResponse(const HTTPResponse &other) = delete;
	HTTPResponse &operator=(const HTTPResponse &rhs) = delete;
	~HTTPResponse();

	ClientState send(int client_fd, const std::string &response);
	void append(const std::string &content);
	void clear(void);
};

#endif
