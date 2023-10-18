#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <HTTPRequest.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

// DEFINES

#ifndef HTTP_WRITE_SIZE
#define HTTP_WRITE_SIZE 1024
#endif

// CLASS
class HTTPResponse
{
  private:
	int _status_code;
	std::string _response;

  public:
	HTTPResponse(const HTTPRequest &request);
	~HTTPResponse();

	HTTPResponse();
	HTTPResponse(const HTTPResponse &other);
	HTTPResponse &operator=(const HTTPResponse &other) = delete;

	int getStatusCode(void) const;
	std::string getHTTPResponse(void) const;
	void createHTTPResponse();
};

#endif
