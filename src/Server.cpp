#include <Logger.hpp>
#include <Server.hpp>
#include <arpa/inet.h>

Server::~Server()
{
}

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

int Server::acceptConnection(const pollfd &pollfd)
{
	Socket client(pollfd.fd);
	Logger &logger = Logger::getInstance();
	char address[INET_ADDRSTRLEN];

	logger.log(INFO, "Accepting connection on server fd: " +
						 std::to_string(_socket.fd));
	client.addr_len = sizeof(client.addr);
	client.fd = accept(fd.fd, (t_sockaddr *)&client.addr,
					   (socklen_t *)&client.addr_len);
	if (client.fd == G_ERROR)
		throw SystemException("Accept");
	inet_ntop(AF_INET, &client.addr.sin_addr, address, sizeof(address));
	logger.log(INFO, "Connection received from " + std::string(address) +
						 " to client fd: " + std::to_string(client.fd));
	return (client.getFD());
}

int Server::getFD(void) const
{
	return (_socket.getFD());
}
