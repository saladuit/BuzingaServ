#ifndef SERVER_HPP
#define SERVER_HPP

#include <ConfigParser.hpp>
#include <ServerSettings.hpp>
#include <Socket.hpp>

class Server
{
  public:
	Server(const ServerSettings &settings);
	Server() = delete;
	Server(const Server &rhs) = delete;
	Server &operator=(const Server &rhs) = delete;
	~Server();
	int getFD(void) const;

  private:
	const ServerSettings &_server_settings;
	Socket _socket;
};

#endif
