#include <Logger.hpp>
#include <Server.hpp>
#include <arpa/inet.h>

Server::~Server()
{
	close(_socket.fd);
}

void Server::_init_sockaddr_in(t_sockaddr_in &addr)
{
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(std::stoi(_port));
	std::fill_n(addr.sin_zero, sizeof(addr.sin_zero), '\0');
}

Server::Server(const ServerBlock &server_block)
	: _host(server_block.settings.at(ServerSetting::Host)),
	  _port(server_block.settings.at(ServerSetting::Port)),
	  _server_name(server_block.settings.at(ServerSetting::ServerName)),
	  _client_max_body_size(
		  server_block.settings.at(ServerSetting::ClientMaxBodySize))
{
	Logger &logger = Logger::getInstance();
	int option = 1;

	_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket.fd == SYSTEM_ERROR)
		throw SystemException("socket creation failed");
	if (setsockopt(_socket.fd, SOL_SOCKET, SO_REUSEADDR, &option,
				   sizeof(option)) == SYSTEM_ERROR)
		throw SystemException("setsockopt failed");
	_init_sockaddr_in(_socket.addr);
	if (bind(_socket.fd, (t_sockaddr *)&_socket.addr, sizeof(t_sockaddr_in)) ==
		SYSTEM_ERROR)
		throw SystemException("Bind");
	if (listen(_socket.fd, MAX_PENDING_CONNECTIONS) == SYSTEM_ERROR)
		throw std::runtime_error("Listen");
	logger.log(DEBUG, "Created Server socket host:name " + _host + ":" + _port +
						  " on fd: " + std::to_string(_socket.fd));
}

int Server::acceptConnection(const pollfd &fd)
{
	t_socket client;
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
	return (client.fd);
}

int Server::getFD(void) const
{
	return (_socket.fd);
}
