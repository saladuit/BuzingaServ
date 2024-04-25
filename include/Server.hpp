#ifndef SERVER_HPP
#define SERVER_HPP

#include <ConfigParser.hpp>
#include <ServerSettings.hpp>
#include <Socket.hpp>

class Server
{
  public:
	Server(const std::vector<ServerSettings> &server_settings);
	Server() = delete;
	Server(const Server &rhs) = delete;
	Server &operator=(const Server &rhs) = delete;
	~Server();

	int getFD(void) const;
	const ServerSettings &getServerSettings(void) const;

  private:
	const std::vector<ServerSettings> _server_settings;
	Socket _socket;
};

#endif
