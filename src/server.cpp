#include "server.hpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

const char	*getResponse(t_method_index methodIndex)
{
	const char	*getResponse = "GET RESPONSE !!!";
	const char	*postResponse = "POST RESPONSE ???";
	const char	*delResponse = "DELETE RESPONSE >>>>>>>>>";
	const char	*errorResponse = "!@! ERROR !@!";
	if (methodIndex == GET)
		return (getResponse);
	else if (methodIndex == POST)
		return (postResponse);
	else if (methodIndex == DELETE)
		return (delResponse);
	else
		return (errorResponse);
}

t_method_index	getMethodIndex(char *request)
{
	const char		*get = "GET";
	const char		*post = "POST";
	const char		*del = "DELETE";

	if (strncmp(request, get, strlen(get)) == 0)
		return (GET);
	else if (strncmp(request, post, strlen(post)) == 0)
		return (POST);
	else if (strncmp(request, del, strlen(del)) == 0)
		return (DELETE);
	else
		return (ERROR);
}

void	handleClient(int clientSocket)
{
	t_method_index	indexMethod;
//	char 			*response;

	// it might be necessary to free buffer here
	const char *message = "Hello from my first simple server!\n";
	char buffer[1024] = {0};

	if (send(clientSocket, message, strlen(message), 0) == -1)
		printf("SERVER: send failed\n");

	if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1)
		printf("SERVER: recv failed\n");

	std::cout << "Received from client:\n" << buffer << std::endl;

	indexMethod = getMethodIndex(buffer);

	if (send(clientSocket, getResponse(indexMethod),
			 strlen(getResponse(indexMethod)), 0) == -1)
		printf("SERVER: send failed\n");
	close(clientSocket);
}

static bool	parse_default_config_file(uint32_t *port_num)
{
	(void)port_num;
	std::ifstream fd("conf/my_config.conf");
	if (!fd.is_open())
	{
		std::cerr << "Error opening config file: conf/my_config.conf" << std::endl;
		return (false);
	}

	std::string line;
	std::string portStr;
	while (std::getline(fd, line))
	{
		size_t pos = line.find("listen");
		if (pos != std::string::npos)
		{
			std::string afterListen = line.substr(pos + 6);
			uint32_t i  = 1;
			while (i < afterListen.length() && std::isdigit(afterListen[i]))
			{
				i++;
			}
			portStr = afterListen.substr(0, i);
			std::cout << "Port number: " << portStr << std::endl;
		}
	}
	if (!portStr.empty()) {
		*port_num = std::stoul(portStr);
	} else {
		std::cerr << "Port number not found in the configuration." << std::endl;
		return false;
	}
	fd.close();
	return (true);
}

int main(int argc, char **argv)
{
	int serverSocket;
	int clientSocket;

	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

	uint32_t port_num = 0;

	if (argc != 1 && argc != 2)
	{
		std::cerr << "SERVER: Invalid number of arguments\n";
		return 1;
	}
	 if (argc == 1)
	 {
		parse_default_config_file(&port_num);
	 }

	// Parse configuration file
	(void)argv;

	socklen_t clientAddrLen = sizeof(clientAddr);

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "SERVER: Error creating socket\n";
		return 1;
	}

	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(port_num); // Choose any available port

	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (struct sockaddr *)&serverAddr,
			 sizeof(serverAddr)) == -1)
	{
		std::cerr << "SERVER: Error binding to port\n";
		close(serverSocket);
		return 1;
	}

	if (listen(serverSocket, 5) == -1)
	{
		std::cerr << "SERVER: Error listening\n";
		close(serverSocket);
		return 1;
	}

	std::cout << "Server listening on port " << std::to_string(port_num)
			  << "...\n\n";

	// try to do something with the clients message
	// -> send something back for example or an error code if the request cannot
	// be handled
	while (true)
	{
		clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr,
							  &clientAddrLen);
		if (clientSocket == -1)
		{
			std::cerr << "SERVER: Error accepting connection\n";
			continue;
		}
		handleClient(clientSocket);
	}

	close(serverSocket);
	return 0;
}
