#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

// INCLUDE
#include <iostream>
#include <map>

// DEFINES
#define _GET "GET"
#define _POST "POST"
#define _DELETE "DELETE"

// ENUM
enum class HTTPMethod {
	GET,
	POST,
	DELETE,
	UNKNOWN,
};

// CLASS
class HTTPRequest {
private:
    int 								_methodType;
	std::string							_path;
	std::string							_version;
	std::map<std::string, std::string>	_headers;
	std::string							_body;

public:
	// canonical form: constructors, operator and destructor
	HTTPRequest();
	HTTPRequest(const HTTPRequest& other) = delete;
	HTTPRequest&	operator=(const HTTPRequest& other) = delete;
	~HTTPRequest();

	// methodType methods
	void	setMethodType(const std::string& requestLine);
	int 	getMethodType(void) const;

	// path methods
	void				setPath(const std::string& requestLine);
	const std::string& 	getPath(void) const;

	// version methods
	void				setVersion(const std::string& requestLine);
	const std::string& 	getVersion(void) const;
};

#endif
