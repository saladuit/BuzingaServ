#include <Logger.hpp>
#include <Server.hpp>
#include <ServerSettings.hpp>
#include <vector>

Server::Server(const std::vector<ServerSettings> &server_settings)
	: _server_settings(server_settings), _socket()
{
	Logger &logger = Logger::getInstance();

	_socket.setupServer(_server_settings.at(0).getListen());
	logger.log(DEBUG, "Created Server on host:port " +
						  _server_settings.at(0).getListen() +
						  " on fd: " + std::to_string(_socket.getFD()));
}

Server::~Server()
{
}

const ServerSettings &Server::getServerSettings(void) const
{
	return (_server_settings);
}

int Server::getFD(void) const
{
	return (_socket.getFD());
}
