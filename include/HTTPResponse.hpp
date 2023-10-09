#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

// INCLUDE
#include <iostream>
#include <map>
#include <HTTPRequest.hpp>

// CLASS
class HTTPResponse {
private:
	std::string							_http_response_str;
	std::string						    _version;
    int                                 _status_code;
    std::string                         _response_phrase;
	std::map<std::string, std::string>	_headers;
	std::string						    _body;

public:
	HTTPResponse(std::string version, int status_code, std::string _body);
	HTTPResponse(const HTTPResponse& other) = delete;
	HTTPResponse&	operator=(const HTTPResponse& other) = delete;
	~HTTPResponse();

	// version methods
	void				setVersion(const std::string& requestLine);
	const std::string& 	getVersion(void) const;

	// header methods
	void				setHeader(const std::string& key, const std::string& value);
	std::string&		getValue(const std::string& key);

	// body methods
	void				setBody(const std::string& body);
	const std::string& 	getBody(void) const;

    int 				getStatusCode(void) const;

	std::string			getHTTPResponse(void) const;
	std::string			getResponsePhrase(void) const;
    void                createHTTPResponse();
};

#endif
