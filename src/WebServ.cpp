#include <WebServ.hpp>

void WebServ::_check(int exp, const char *msg)
{
	if (exp == ERROR)
	{
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

WebServ::WebServ(const std::string &config_path)
	: _config_path(config_path), _address_len(sizeof(t_sockaddr_in)),
	  _server_socket(socket(AF_INET, SOCK_STREAM, 0))
{

	_check(_server_socket, "socket creation failed");
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htons(INADDR_ANY);
	_address.sin_port = htons(SERVER_PORT);
	std::fill_n(_address.sin_zero, sizeof(_address.sin_zero), '\0');
	_check(bind(_server_socket, (t_sockaddr *)&_address, _address_len),
		   "bind failed");
	_check(listen(_server_socket, 3), "listen failed");
}

WebServ::~WebServ()
{
}

int WebServ::run()
{
	while (true)
	{
		std::cout << "Waiting for connection...\n";
		int new_socket = accept(_server_socket, (struct sockaddr *)&_address,
								(socklen_t *)&_address_len);
		if (new_socket < 0)
		{
			std::cerr << "Error: accept failed" << std::endl;
			return (EXIT_FAILURE);
		}
		// Read and write data on a socket
		char buffer[BUFFER_SIZE] = {0};
		int valread = read(new_socket, buffer, 1024);
		if (valread < 0)
		{
			std::cerr << "Error: read failed" << std::endl;
			return (EXIT_FAILURE);
		}
		printf("%s\n", buffer);
		std::string hello = "Hello from server";
		// write(new_socket, hello.c_str(), strlen(hello.c_str()));
		close(new_socket);
	}
	return (EXIT_SUCCESS);
}
