#ifndef SERVER_HPP
#define SERVER_HPP

#include <ConfigParser.hpp>
#include <Defines.hpp>
#include <Socket.hpp>

#include <sys/poll.h>
#include <unistd.h>

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
	const std::string &_host;
	const std::string &_port;
	const std::string &_server_name;
	const std::string &_client_max_body_size;
	Socket _socket;
	void _init_sockaddr_in(t_sockaddr_in &addr);
};

#endif
