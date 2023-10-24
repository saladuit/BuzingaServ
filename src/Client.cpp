#include "ClientException.hpp"
#include "HTTPStatus.hpp"
#include <Client.hpp>

#include <sys/poll.h>

Client::Client(const int &server_fd) : _socket(server_fd)
{
}

Client::~Client()
{
}

int Client::getFD(void) const
{
	return (_socket.getFD());
}

ClientState Client::handleConnection(short events)
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "Handling client connection on fd: " +
						 std::to_string(_socket.getFD()));
	try
	{
		if (events & POLLIN)
			return (_request.receive(_socket.getFD()));
		else if (events & POLLOUT && _state == ClientState::Loading)
		{
			_file_manager.manage(_request.getMethodType(),
								 _request.getRequestTarget(),
								 _request.getBody());
		}
		else if (events & POLLOUT && _state == ClientState::Error)
			_file_manager.loadErrorPage();
		else if (events & POLLOUT && _state == ClientState::Sending)
			return (
				_response.send(_socket.getFD(), _file_manager.getResponse()));
	}
	catch (ClientException &e)
	{
		// TODO: Add clear function
		_response.append(e.what()); // TODO: Error pages
		_file_manager.openErrorPage(e.getStatusCode());
		_state = ClientState::Error;
	}
	return (ClientState::Unkown);
}
