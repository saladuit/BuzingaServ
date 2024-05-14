#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

// MAX_PENDING_CONNECTION SHOULDN'T BE 1
// WHY 10? WE CAN ALSO SET IT ON 1024, THAT'S
// MORE SAFE, CONCERNING THE AVAILABILITY (siege -b)
#define MAX_PENDING_CONNECTIONS 10

typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr t_sockaddr;

class Socket
{
  private:
	socklen_t _addr_len;
	t_sockaddr_in _addr;
	const int _fd;
	void initSockaddrIn(t_sockaddr_in &addr, const std::string &_port);

  public:
	Socket();
	Socket(int server_fd);
	Socket(const Socket &other) = delete;
	Socket &operator=(const Socket &other) = delete;
	~Socket();
	int getFD() const;
	void setupServer(const std::string &port);
	void setupClient(void);
};

#endif
