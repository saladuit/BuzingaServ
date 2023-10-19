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
		return (_request.receive(_socket.getFD()));
	if (events & POLLOUT)
		return (_response.send(_socket.getFD()));
	return (ClientState::Unkown);
}
