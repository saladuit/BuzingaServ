#include "server.hpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void handleClient(int clientSocket)
{
	// it might be necessary to free buffer here
	const char *message = "Hello from my first simple server!\n";
	char buffer[1024] = {0};

	if (send(clientSocket, message, strlen(message), 0) == -1)
		printf("SERVER: send failed\n");
	else
		printf("SERVER: send succeeded\n");

	if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1)
		printf("SERVER: recv failed\n");
	else
		printf("SERVER: recv succeeded\n");

	std::cout << "Received from client: " << buffer << std::endl;

	const char	*response = "test";
	if (strncmp(buffer, response, strlen(response)) == 0)
	{
		if (send(clientSocket, response, strlen(response), 0) == -1)
			printf("SERVER: send failed\n");
		else {
			printf("SERVER: send succeeded\n");
			close(clientSocket);
		}
	}
}

int main()
{
	int serverSocket;
	int clientSocket;

	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

	socklen_t clientAddrLen = sizeof(clientAddr);

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "SERVER: Error creating socket\n";
		return 1;
	}

	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(PORT_NUM); // Choose any available port

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

	std::cout << "Server listening on port " << std::to_string(PORT_NUM)
			  << "...\n";

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
