#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <ClientState.hpp>

#include <string>
#include <unordered_map>

#define BUFFER_SIZE 64

// DEFINES

#ifndef HTTP_READ_SIZE
#define HTTP_READ_SIZE 1024
#endif

// ENUM
enum class HTTPMethod
{
	GET,
	POST,
	DELETE,
	UNKNOWN,
};

// CLASS
class HTTPRequest
{
  public:
	HTTPRequest();
	HTTPRequest(const HTTPRequest &rhs) = delete;
	HTTPRequest &operator=(const HTTPRequest &other) = delete;
	~HTTPRequest();

	void setMethodType(const std::string &method_type);
	HTTPMethod getMethodType(void) const;

	void setRequestTarget(const std::string &request_target);
	const std::string &getRequestTarget(void) const;

	void setHTTPVersion(const std::string &http_version);
	const std::string &getHTTPVersion(void) const;

	void setHeader(const std::string &key, const std::string &header);
	const std::string &getHeader(const std::string &key) const;

	const std::string &getBody(void) const;
	ClientState receive(int fd);

	// CGI stuff
	void				parseURIForCGI(void);
	const std::string	&getExecutable(void) const;
	char				**getEnv(void);
	void				setCGIToTrue(void);
	const bool			&CGITrue(void) const;
	const size_t		&getBodyLength(void) const;

	

  private:
	ssize_t _bytes_read;
	size_t _content_length;
	HTTPMethod _methodType;
	std::string _http_request;
	std::string _request_target;
	std::string _http_version;
	std::string _body;
	std::unordered_map<std::string, std::string> _headers;
	std::string	_executable;
	char		**_env;
	bool		_cgi;

	size_t parseStartLine(size_t &i);
	size_t parseHeaders(size_t &i);
};

#endif
