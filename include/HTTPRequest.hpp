#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <ClientState.hpp>
#include <ServerSettings.hpp>

#include <string>
#include <unordered_map>

#define BUFFER_SIZE 64

// DEFINES

#ifndef HTTP_READ_SIZE
#define HTTP_READ_SIZE 1024
#endif

// ENUM
#ifndef HTTP_METHOD_ENUM
#define HTTP_METHOD_ENUM
enum class HTTPMethod
{
	GET,
	POST,
	DELETE,
	UNKNOWN,
};
#endif

// CLASS
class HTTPRequest
{
  public:
	HTTPRequest();
	HTTPRequest(const HTTPRequest &rhs) = delete;
	HTTPRequest &operator=(const HTTPRequest &rhs) = delete;
	~HTTPRequest();

	void setMethodType(const std::string &method_type);
	HTTPMethod getMethodType(void) const;

	void setRequestTarget(const std::string &request_target);
	const std::string &getRequestTarget(void) const;

	void setMaxBodySize(std::string inp);
	ssize_t getMaxBodySize(void) const;

	void setHTTPVersion(const std::string &http_version);
	const std::string &getHTTPVersion(void) const;

	void setHeader(const std::string &key, const std::string &header);
	const std::string &getHeader(const std::string &key) const;

	const std::string &getBody(void) const;
	ClientState setRequestVariables(size_t pos);
	ClientState receive(int fd);

	void setHeaderEnd(bool b);
	bool getHeaderEnd() const;

  private:
	bool _header_end;
	ssize_t _bytes_read;
	size_t _content_length;
	size_t _max_body_size;
	HTTPMethod _methodType;
	std::string _http_request;
	std::string _request_target;
	std::string _http_version;
	std::string _body;
	std::unordered_map<std::string, std::string> _headers;

	size_t parseStartLine(size_t &i);
	size_t parseHeaders(size_t &i);
	void setLocationdependancies(std::string request_target);
};

#endif
