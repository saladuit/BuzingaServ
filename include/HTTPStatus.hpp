#ifndef HTTP_ERROR_HPP
#define HTTP_ERROR_HPP

#include <StatusCode.hpp>

#include <string>
#include <unordered_map>

class HTTPStatus
{
  private:
	StatusCode _status_code;
	static std::unordered_map<StatusCode, std::string> _message;

  public:
	HTTPStatus() = delete;
	HTTPStatus(StatusCode status_code);
	HTTPStatus(const HTTPStatus &other) = delete;
	HTTPStatus &operator=(const HTTPStatus &other) = delete;
	~HTTPStatus();

	std::string getStatusLine(const std::string &version) const;
	StatusCode getStatusCode() const;
};

#endif
