#ifndef RETURN_EXCEPTION_HPP
#define RETURN_EXCEPTION_HPP

#include "LocationSettings.hpp"
#include <HTTPStatus.hpp>
#include <stdexcept>

class ReturnException : public HTTPStatus, public std::runtime_error
{
  private:
	const std::string _redireciton;

  public:
	ReturnException(StatusCode status, const LocationSettings &locationBlock)
		: HTTPStatus(status), std::runtime_error(getStatusLineCRLF("HTTP/1.1")),
		  _redireciton(locationBlock.getRedirect()){};

	const std::string &getRedirection(void)
	{
		return (_redireciton);
	}
};

#endif
