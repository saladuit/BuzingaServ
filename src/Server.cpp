#include "ServerSetting.hpp"
#include <Logger.hpp>
#include <Server.hpp>

Server::Server(const ServerSetting &server_setting)
	: _host(server_setting.getServerSetting(ServerSettingOption::Host)),
	  _port(server_setting.getServerSetting(ServerSettingOption::Port)),
	  _server_name(
		  server_setting.getServerSetting(ServerSettingOption::ServerName)),
	  _client_max_body_size(server_setting.getServerSetting(
		  ServerSettingOption::ClientMaxBodySize)),
	  _error_pages(
		  server_setting.getServerSetting(ServerSettingOption::ErrorPages)),
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
