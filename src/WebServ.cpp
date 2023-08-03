#include <WebServ.hpp>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

WebServ::WebServ(const std::string &config_path) : _config_path(config_path)
{
}

WebServ::~WebServ()
{
}

#define PORT 8080

int WebServ::run()
{
	// Create a socket for TCP/IP connection
	struct sockaddr_in address;
	int address_len = sizeof(address);
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1)
	{
		std::cerr << "Error: socket creation failed" << std::endl;
		return (EXIT_FAILURE);
	}
	// Assign a transport address to the socket
	// address family we use
	address.sin_family = AF_INET;
	// OS decide which ip to use 0.0.0.0
	address.sin_addr.s_addr = INADDR_ANY;
	// convert port number to network byte order
	address.sin_port = htons(PORT);
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	// Bind a name to a socket
	if (bind(server_fd, (struct sockaddr *)&address, address_len) < 0)
	{
		std::cerr << "Error: bind failed" << std::endl;
		return (EXIT_FAILURE);
	}
	// Listen for connections on a socket
	if (listen(server_fd, 3) < 0)
	{
		std::cerr << "Error: listen failed" << std::endl;
		return (EXIT_FAILURE);
	}
	while (true)
	{
		std::cout << "Waiting for connection...\n";
		// Accept a connection on a socket
		int new_socket = accept(server_fd, (struct sockaddr *)&address,
								(socklen_t *)&address_len);
		if (new_socket < 0)
		{
			std::cerr << "Error: accept failed" << std::endl;
			return (EXIT_FAILURE);
		}
		// Read and write data on a socket
		char buffer[1024] = {0};
		int valread = read(new_socket, buffer, 1024);
		if (valread < 0)
		{
			std::cerr << "Error: read failed" << std::endl;
			return (EXIT_FAILURE);
		}
		printf("%s\n", buffer);
		std::string hello = "Hello from server";
		write(new_socket, hello.c_str(), strlen(hello.c_str()));
		close(new_socket);
	}
	return (EXIT_SUCCESS);
}
