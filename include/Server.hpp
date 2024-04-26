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
	std::vector<ServerSettings> &getServerSettings(void);

  private:
	std::vector<ServerSettings> _server_settings;
	Socket _socket;
};

#endif
