#include <HTTPServer.hpp>

void HTTPServer::_check(int exp, const char *msg)
{
	if (exp == G_ERROR)
	{
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

void HTTPServer::_setup_server_socket()
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

HTTPServer::HTTPServer()
{
}

void HTTPServer::handle_connection()
{
	size_t bytes_read;
	int msgsize = 0;
	std::fill_n(_buffer, sizeof(_buffer), '\0');
	std::fill_n(_actual_path, sizeof(_actual_path), '\0');
	do
	{
		_check(bytes_read = read(_client.fd, _buffer + msgsize,
								 sizeof(_buffer) - msgsize - 1),
			   "Error: read failed");
		msgsize += bytes_read;

		if (msgsize > BUFFER_SIZE - 1 || _buffer[msgsize - 1] == '\n')
			break;
		std::fill_n(_buffer, sizeof(_buffer), '\0');
	} while (bytes_read > 0);

	_buffer[msgsize - 1] = 0;
	std::cout << "REQUEST: " << _buffer << std::endl;
	if (realpath(_buffer, _actual_path) == NULL)
	{
		std::cout << "Error: realpath failed" << std::endl;
		close(_client.fd);
		return;
	}
	FILE *file = fopen(_actual_path, "r");
	if (file == NULL)
	{
		std::cout << "Error: fopen failed" << std::endl;
		close(_client.fd);
		return;
	}
	// while (bytes_read = fread(_buffer, 1, sizeof(_buffer), file) > 0)
	// {
	// 	std::cout << "Sending: " << bytes_Read << " bytes" << std::endl;
	// 	write(_client.fd, _buffer, bytes_read);
	// }
	std::fill_n(_buffer, sizeof(_buffer), '\0');
	snprintf(_buffer, sizeof(_buffer), "HTTP/1.1 200 OK\r\n\r\nHello");
	write(_client.fd, _buffer, strlen(_buffer));
	close(_client.fd);
}

void HTTPServer::accept_connection()
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

int HTTPServer::run()
{
	_setup_server_socket();
	while (true)
	{
		accept_connection();
		handle_connection();
	}
	return (EXIT_SUCCESS);
}
