#ifndef Server_HPP
#define Server_HPP

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

class Server
{
  public:
	Server(const std::string &config_path);
	~Server();

	int run();

  private:
	Server();
	Server(const Server &src);
	Server &operator=(const Server &rhs);

	void _setup_server_socket();
	void handle_connection();
	void accept_connection();

	void _check(int exp, const char *msg);

	const std::string _config_path;

	t_socket _server;
	t_socket _client;
	char _buffer[BUFFER_SIZE];
	char _actual_path[BUFFER_SIZE];
};
#endif

/* ************************************************************************** */
