#ifndef CLIENT_EXCEPTION_HPP
#define CLIENT_EXCEPTION_HPP

#include <HTTPStatus.hpp>
#include <stdexcept>

class ClientException : public HTTPStatus, public std::runtime_error
{
  public:
	ClientException(StatusCode status)
		: HTTPStatus(status), std::runtime_error(getStatusLine("HTTP/1.1")){};
};

#endif
