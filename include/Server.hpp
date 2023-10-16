#ifndef SERVER_HPP
#define SERVER_HPP

#include <ConfigParser.hpp>
#include <Defines.hpp>
#include <SystemException.hpp>

#include <netinet/in.h>
#include <sys/poll.h>
#include <unistd.h>

#define SYSTEM_ERROR (-1)

typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr t_sockaddr;

typedef struct s_socket
{
	int fd;
	t_sockaddr_in addr;
	size_t addr_len;
} t_socket;

class Server
{
  public:
	Server(const ServerBlock &server_block);
	Server() = delete;
	Server(const Server &src) = delete;
	Server &operator=(const Server &rhs) = delete;
	~Server();
	int getFD() const;
	int acceptConnection(const pollfd &fd);

  private:
	void _init_sockaddr_in(t_sockaddr_in &addr);
	const std::string &_host;
	const std::string &_port;
	const std::string &_server_name;
	const std::string &_client_max_body_size;
	t_socket _socket;
};

#endif
