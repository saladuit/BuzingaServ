#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>

#include <string>
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

void HTTPResponse::clear(void)
{
	_bytes_sent = 0;
	_response.clear();
}

ClientState HTTPResponse::send(int client_fd, const std::string &response)
{
	Logger &logger = Logger::getInstance();
	ssize_t bytes_to_send;
	ssize_t w_size;

	if (_response.empty())
		_response = response;
	logger.log(INFO, "Sending response to client on fd: " +
						 std::to_string(client_fd));
	//	logger.log(DEBUG, "response: " + _response); // this logger line gives a
	//	segfault when transporting /data/images/conffee-resized.jpeg
	if (_response.length() - _bytes_sent < BUFFER_SIZE)
		bytes_to_send = _response.length() - _bytes_sent;
	else
		bytes_to_send = BUFFER_SIZE;
	w_size =
		write(client_fd, _response.substr(_bytes_sent).c_str(), bytes_to_send);
	if (w_size == -1)
		throw SystemException("Error: write failed on: " +
							  std::to_string(client_fd)); // TODO: handle error
	_bytes_sent += w_size;
	if (_bytes_sent == _response.length())
	{
		clear();
		return (ClientState::Done);
	}
	return (ClientState::Sending);
}
