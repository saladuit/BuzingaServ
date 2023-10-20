#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <ClientState.hpp>
#include <HTTPRequest.hpp>
#include <fstream>
#include <string>

class HTTPResponse
{
  private:
	ssize_t _bytes_sent;
	std::string _response;
	std::ifstream _requested_file;

  public:
	HTTPResponse();
	HTTPResponse(const HTTPResponse &other) = delete;
	HTTPResponse &operator=(const HTTPResponse &rhs) = delete;
	~HTTPResponse();

	ClientState send(int client_fd);
	void composeStartLine(int status, const std::string &file_content);
};

#endif
