#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

// PARSE_HTTP_REQUEST -- request struct?		Status:
// 1) parse request line						done
// 		a) Set method type -- enum				done
// 		b) Set path								done
//		c) Set version							done
// 2) parse headers --> map						done
// 3) parse body --> ??							done (as string)

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
class HTTPRequest {
private:
	HTTPMethod							_methodType;
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
	void		setMethodType(const std::string& requestLine);
	HTTPMethod	getMethodType(void) const;

	// path methods
	void				setPath(const std::string& requestLine);
	const std::string& 	getPath(void) const;

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
	void	parse(const std::string& HTTPRequestString);
};

#endif
