#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

// CREATE_HTTP_RESPONSE 		Status:                 from:
// 1) request line	        	                        
// 		a) Version          	                        http_request
// 		b) status_code   	                            http_request / file_manager
//		c) Reason_phrase		                        status_code --> reason_phrase
// 2) headers --> map									create in class / http_manager
// 3) body --> ??		    							file_manager (only for a GET request)

// EXAMPLES 
// HTTP/1.1 200 OK
// Date: Mon, 27 Jul 2009 12:28:53 GMT
// Server: Apache/2.2.14 (Win32)
// Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
// Content-Length: 88
// Content-Type: text/html
// Connection: Closed

// HTTP/1.1 404 Not Found
// Date: Sun, 18 Oct 2012 10:36:20 GMT
// Server: Apache/2.2.14 (Win32)
// Content-Length: 230
// Connection: Closed
// Content-Type: text/html; charset=iso-8859-1

// INCLUDE
#include <iostream>
#include <map>

// DEFINES

// ENUM
enum class HTTPMethod {
	GET,
	POST,
	DELETE,
	UNKNOWN,
};

// CLASS
class HTTPResponse {
private:
	HTTPMethod							_methodType;
	std::string							_path;
	std::string							_version;
	std::map<std::string, std::string>	_headers;
	std::string							_body;

public:
	// canonical form: constructors, operator and destructor
	HTTPResponse();
	HTTPResponse(const HTTPResponse& other) = delete;
	HTTPResponse&	operator=(const HTTPResponse& other) = delete;
	~HTTPResponse();

	// methodType methods -- NOT NECESSARY I GUESS
	// void		setMethodType(const std::string& requestLine);
	// HTTPMethod	getMethodType(void) const;

	// path methods -- NOT NECESSARY
	// void				setPath(const std::string& requestLine);
	// const std::string& 	getPath(void) const;

	// version methods
	void				setVersion(const std::string& requestLine);
	const std::string& 	getVersion(void) const;

	// header methods
	void				setHeader(const std::string& headerLine);
	std::string&		getValue(const std::string& key);

	// body methods
	void				setBody(const std::string& body);
	const std::string& 	getBody(void) const;

	// parser
	// void	parse(const std::string& HTTPResponseString);
};

#endif