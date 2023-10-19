#include <Logger.hpp>
#include <Server.hpp>

Server::Server(const ServerBlock &server_block)
	: _host(server_block.settings.at(ServerSetting::Host)),
	  _port(server_block.settings.at(ServerSetting::Port)),
	  _server_name(server_block.settings.at(ServerSetting::ServerName)),
	  _client_max_body_size(
		  server_block.settings.at(ServerSetting::ClientMaxBodySize)),
	  _socket()
{
	Logger &logger = Logger::getInstance();

	_socket.setupServer(_port);
	logger.log(DEBUG, "Created Server on host:port " + _host + ":" + _port +
						  " on fd: " + std::to_string(_socket.getFD()));
}

Server::~Server()
{
}

int Server::getFD(void) const
{
	return (_socket.getFD());
}
