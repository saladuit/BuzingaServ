#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>

class Socket
{
  private:
	int _fd;
	char _address[INET_ADDRSTRLEN];

  public:
	Socket(int domain, int type);
	Socket(int fd);
	Socket() = delete;
	Socket(const Socket &other) = delete;
	Socket &operator=(const Socket &other) = delete;
	~Socket();
	int getFD() const;
};

#endif
