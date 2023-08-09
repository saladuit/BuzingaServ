#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

// readline
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "server.hpp"

int main()
{
	int clientSocket;

	// this fixed buffer size could lead to crashes
	char buffer[1024] = {0};
	struct sockaddr_in serverAddr;
	int port = PORT_NUM;
//	const char *message = "Hello from client!\n";

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1)
	{
		std::cerr << "CLIENT: Error creating socket\n";
		return 1;
	}

	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(port);

	inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));
	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		std::cerr << "CLIENT: Error creating socket\n";
		return 1;
	}

	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << "Received from server: " << buffer << std::endl;

	while (true)
	{
		char *something = readline("Type something for the server: ");
		//	send(clientSocket, message, strlen(message), 0);
		if (strncmp(something, "STOP\0", 5) == 0)
		{
			free(something);
			break ;
		}
		if (send(clientSocket, something, strlen(something), 0) == -1)
			printf("CLIENT: send failed\n");
		else
			printf("CLIENT: send succeeded\n");
		free(something);

		char responseBuffer[1024] = {0};
		recv(clientSocket, responseBuffer, sizeof(responseBuffer), 0);
		std::cout << "Received from server: " << responseBuffer << std::endl;
	}
	close(clientSocket);
	return 0;
}
