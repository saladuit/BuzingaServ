#include <Logger.hpp>
#include <Socket.hpp>
#include <SystemException.hpp>

#include <arpa/inet.h>
#include <cassert>
#include <unistd.h>

Socket::Socket(const int fd)
	: _addr_len(sizeof(_addr)),
	  _fd(accept(fd, (t_sockaddr *)&_addr, &_addr_len))
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "Accepting connection on fd: " + std::to_string(fd));
	if (_fd == SYSTEM_ERROR)
		throw SystemException("Accept");
	char address[INET_ADDRSTRLEN];
	assert(inet_ntop(AF_INET, &_addr.sin_addr, address, sizeof(address)) !=
		   NULL);
	logger.log(INFO,
			   "Connection received from " + std::string(address) +
				   " created client socket on fd: " + std::to_string(getFD()));
}

Socket::Socket(const Socket &other)
	: _addr_len(other._addr_len), _addr(other._addr), _fd(other._fd)
{
}

Socket::Socket() : _fd(socket(AF_INET, SOCK_STREAM, 0))
{
	if (_fd == SYSTEM_ERROR)
		throw SystemException("socket creation failed");
}

Socket::~Socket()
{
	close(_fd);
}

void Socket::initSockaddrIn(t_sockaddr_in &addr, const std::string &_port)
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
	initSockaddrIn(_addr, port);
	if (bind(_fd, (t_sockaddr *)&_addr, sizeof(t_sockaddr_in)) == SYSTEM_ERROR)
		throw SystemException("Bind");
	if (listen(_fd, MAX_PENDING_CONNECTIONS) == SYSTEM_ERROR)
		throw SystemException("Listen");
}

int Socket::getFD() const
{
	return (_fd);
}
