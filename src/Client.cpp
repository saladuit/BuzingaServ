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
	if (events & POLLIN)
	{
		_state = _request.receive(_socket.getFD());
		if (_state == ClientState::Loading)
			/* _file_manager.openFile(_request.getRequestTarget()); */
			return (_state);
	}
	if (events & POLLOUT && _state == ClientState::Loading)
	{
		/* _response.append(_file_manager.loadFile()); */
	}
	if (events & POLLOUT && _state == ClientState::Sending)
	{
		_state = _response.send(_socket.getFD());
		return (_state);
	}
	return (ClientState::Unkown);
}
