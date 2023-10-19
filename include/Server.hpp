#ifndef SERVER_HPP
#define SERVER_HPP

#include <ConfigParser.hpp>
#include <Socket.hpp>

class Server
{
  public:
	Server(const ServerSetting &server_block);
	Server() = delete;
	Server(const Server &src) = delete;
	Server &operator=(const Server &rhs) = delete;
	~Server();
	int getFD(void) const;

  private:
	const std::string &_host;
	const std::string &_port;
	const std::string &_server_name;
	const std::string &_client_max_body_size;
	const std::string &_error_pages;
	Socket _socket;
};

#endif
