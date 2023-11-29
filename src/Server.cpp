#include <Logger.hpp>
#include <Server.hpp>
#include <ServerSettings.hpp>

Server::Server(const ServerSettings &server_settings)
	: _server_settings(server_settings), _socket()
{
	Logger &logger = Logger::getInstance();
	const std::string &host =
		server_settings.getValue(ServerSettingOption::Host);
	const std::string &port =
		server_settings.getValue(ServerSettingOption::Port);

	std::string listen_directives = server_settings.getListen();
	_socket.setupServer(host, port);
	logger.log(DEBUG, "Created Server on host:port " +
						  _server_settings.getValue(ServerSettingOption::Host) +
						  ":" +
						  _server_settings.getValue(ServerSettingOption::Port) +
						  " on fd: " + std::to_string(_socket.getFD()));
}

Server::~Server()
{
}

int Server::getFD(void) const
{
	return (_socket.getFD());
}
