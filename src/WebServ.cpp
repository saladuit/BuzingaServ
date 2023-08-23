#include <WebServ.hpp>

void WebServ::_check(int exp, const char *msg)
{
	if (exp == ERROR)
	{
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

WebServ::WebServ(const std::string &config_path) : _config_path(config_path)
{
	_check(_server.fd = socket(AF_INET, SOCK_STREAM, 0),
		   "Error: socket creation failed");
	bzero(&_server.addr, sizeof(_server.addr));
	_server.addr.sin_family = AF_INET;
	_server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_server.addr.sin_port = htons(SERVER_PORT);
	_server.addr_len = sizeof(_server.addr);
	std::fill_n(_server.addr.sin_zero, sizeof(_server.addr.sin_zero), '\0');
	_check(bind(_server.fd, (t_sockaddr *)&_server.addr, _server.addr_len),
		   "Error: bind failed");
	_check(listen(_server.fd, MAX_PENDING_CONNECTIONS), "Error: listen failed");
}

WebServ::~WebServ()
{
}

void WebServ::handle_connection()
{
	int valread;
	std::fill_n(_recvline, sizeof(_recvline), '\0');
	do
	{
		_check(valread = read(_client.fd, _recvline, sizeof(_recvline) - 1),
			   "Error: read failed");
		std::cout << "Received: " << _recvline << std::endl;
		if (_recvline[valread - 1] == '\n') // http req ends in /r/n/r/n
			break;
		std::fill_n(_recvline, sizeof(_recvline), '\0');
	} while (valread > 0);
	snprintf(_buffer, sizeof(_buffer), "HTTP/1.0 200 OK\r\n\r\nHello");
	write(_client.fd, _buffer, strlen(_buffer));
	close(_client.fd);
}

void WebServ::accept_connection()
{
	char client_address[BUFFER_SIZE + 1];
	std::cout << "Waiting for connection on port " << SERVER_PORT << std::endl;
	_client.addr_len = sizeof(_client.addr);
	_check(_client.fd = accept(_server.fd, (t_sockaddr *)&_client.addr,
							   (socklen_t *)&_client.addr_len),
		   "Error: accept failed");
	inet_ntop(AF_INET, &_client.addr.sin_addr, client_address, sizeof(_buffer));
	std::cout << "Connection received from " << client_address << std::endl;
}

int WebServ::run()
{
	while (true)
	{
		accept_connection();
		handle_connection();
	}
	return (EXIT_SUCCESS);
}
