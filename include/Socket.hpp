#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
#include <string>

#define MAX_PENDING_CONNECTIONS 10

typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr t_sockaddr;

class Socket
{
  private:
	socklen_t _addr_len;
	t_sockaddr_in _addr;
	const int _fd;
	void _init_sockaddr_in(t_sockaddr_in &addr, const std::string &_port);

  public:
	Socket();
	Socket(const int server_fd);
	Socket(const Socket &other);
	Socket &operator=(const Socket &other) = delete;
	~Socket();
	int getFD() const;
	void setupServer(const std::string &port);
};

#endif
