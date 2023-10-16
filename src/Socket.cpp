#include "Server.hpp"
#include <Logger.hpp>
#include <Socket.hpp>
#include <SystemException.hpp>

#include <sys/socket.h>
#include <unistd.h>

Socket::Socket(int fd)
	: _fd(accept(fd, (t_sockaddr *)_address, (socklen_t *)sizeof(_address)))
{
	if (_fd == SYSTEM_ERROR)
		throw SystemException("Accept");
}

Socket::Socket(int domain, int type) : _fd(socket(domain, type, 0))
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
