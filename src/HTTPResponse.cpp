#include "HTTPRequest.hpp"
#include <HTTPResponse.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>

#include <unistd.h>

HTTPResponse::HTTPResponse() : _bytes_sent(0), _response("")
{
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::append(const std::string &content)
{
	_response.append(content);
}

ClientState HTTPResponse::send(int client_fd)
{
	Logger &logger = Logger::getInstance();
	ssize_t bytes_to_send;
	ssize_t w_size;

	logger.log(INFO, "Sending response to client on fd: " +
						 std::to_string(client_fd));
	logger.log(DEBUG, "response: " + _response);
	if (_response.length() - _bytes_sent < BUFFER_SIZE)
		bytes_to_send = _response.length() - _bytes_sent;
	else
		bytes_to_send = BUFFER_SIZE;
	w_size =
		write(client_fd, _response.substr(_bytes_sent).c_str(), bytes_to_send);
	if (w_size == -1)
		throw SystemException("Error: write failed on: " +
							  std::to_string(client_fd)); // TODO handle error
	_bytes_sent += w_size;
	return (ClientState::Done);
}
