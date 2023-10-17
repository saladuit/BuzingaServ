#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

// INCLUDE
#include <HTTPRequest.hpp>
#include <iostream>
#include <map>

// DEFINES

#ifndef HTTP_WRITE_SIZE
#define HTTP_WRITE_SIZE 1024
#endif

// CLASS
class HTTPResponse
{
  private:
	std::string _http_response_str;
	std::string _version;
	int _status_code;
	std::string _response_phrase;
	std::map<std::string, std::string> _headers;
	std::string _body;

  public:
	HTTPResponse(std::string version, int status_code, std::string _body);
	HTTPResponse() = delete;
	HTTPResponse(const HTTPResponse &other) = delete;
	HTTPResponse &operator=(const HTTPResponse &other) = delete;
	~HTTPResponse();

	// version methods
	void setVersion(const std::string &requestLine);
	const std::string &getVersion(void) const;

	// header methods
	void setHeader(const std::string &key, const std::string &value);
	std::string &getValue(const std::string &key);

	// body methods
	void setBody(const std::string &body);
	const std::string &getBody(void) const;

	const std::string &getPath(void) const;
	void setPath(const std::string &requestLine);
	int getStatusCode(void) const;

	void setMethodType(const std::string &requestLine);
	std::string getHTTPResponse(void) const;
	std::string getResponsePhrase(void) const;
	void createHTTPResponse();
};

#endif
