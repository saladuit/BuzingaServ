#include <Logger.hpp>
#include <Socket.hpp>
#include <SystemException.hpp>

#include <sys/socket.h>
#include <unistd.h>

Socket::Socket(int fd)
	: _addr_len(sizeof(_address)),
	  _fd(accept(fd, (t_sockaddr *)_address, &_addr_len))
{
	if (_fd == SYSTEM_ERROR)
		throw SystemException("Accept");

	Logger &logger = Logger::getInstance();
	inet_ntop(AF_INET, &client.addr.sin_addr, address, sizeof(address));
	logger.log(INFO, "Created connection on fd: " + std::to_string(_fd));
}

void Socket::_init_sockaddr_in(t_sockaddr_in &addr, const std::string &_port)
{
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(std::stoi(_port));
	std::fill_n(addr.sin_zero, sizeof(addr.sin_zero), '\0');
}

void Socket::setupServer(const std::string &port)
{
	int option = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) ==
		SYSTEM_ERROR)
		throw SystemException("setsockopt failed");
	_init_sockaddr_in(_addr, port);
	if (bind(_fd, (t_sockaddr *)&_addr, sizeof(t_sockaddr_in)) == SYSTEM_ERROR)
		throw SystemException("Bind");
	if (listen(_fd, MAX_PENDING_CONNECTIONS) == SYSTEM_ERROR)
		throw SystemException("Listen");
}
Socket::Socket() : _fd(socket(AF_INET, SOCK_STREAM, 0))
{
	if (_fd == SYSTEM_ERROR)
		throw SystemException("socket creation failed");
}

int Socket::getFD() const
{
	return (_fd);
}

Socket::~Socket()
{
	close(_fd);
}
