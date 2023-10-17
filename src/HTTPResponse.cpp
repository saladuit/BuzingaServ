#include <HTTPResponse.hpp>

HTTPResponse::HTTPResponse()
{
}

HTTPResponse::HTTPResponse(const HTTPRequest &request)
{
	(void)request;
}

HTTPResponse::HTTPResponse(const HTTPResponse &other)
	: _status_code(other._status_code), _response(other._response)
{
}

HTTPResponse::~HTTPResponse()
{
}
