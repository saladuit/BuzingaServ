#include <HTTPResponse.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>

#include <unistd.h>

HTTPResponse::HTTPResponse()
	: _status_code(0), _bytes_sent(0), _response("HTTP/1.1 200 OK\r\n\r\nHello")
{
}

HTTPResponse::~HTTPResponse()
{
}

ClientState HTTPResponse::send(int client_fd)
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "Sending response to client on fd: " +
						 std::to_string(client_fd));
	logger.log(DEBUG, "response: " + _response);
	_bytes_sent += write(client_fd, _response.c_str(), _response.length());
	if (_bytes_sent == -1)
		throw SystemException("Error: write failed on: " +
							  std::to_string(client_fd));
	return (ClientState::Done);
}
