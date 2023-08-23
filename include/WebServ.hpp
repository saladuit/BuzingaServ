#ifndef WebServ_HPP
#define WebServ_HPP

#include <Color.hpp>
#include <Defines.hpp>
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr t_sockaddr;

typedef struct s_socket
{
	int fd;
	t_sockaddr_in addr;
	socklen_t addr_len;
} t_socket;

class WebServ
{
  public:
	WebServ(const std::string &config_path);
	~WebServ();
	int run();

  private:
	void _setup_server_socket(int port);
	void _check(int exp, const char *msg);

	const std::string _config_path;

	t_socket _server;
	t_socket _client;
	char _buffer[BUFFER_SIZE];
	char _recvline[BUFFER_SIZE];
	WebServ();
	WebServ(const WebServ &src);
	WebServ &operator=(const WebServ &rhs);
};
#endif

/* ************************************************************************** */
