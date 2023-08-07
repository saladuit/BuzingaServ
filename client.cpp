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
	char buffer[1024] = {0};
	struct sockaddr_in serverAddr;
	int port = PORT_NUM;
//	const char *message = "Hello from client!\n";

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1)
	{
		std::cerr << "Error creating socket\n";
		return 1;
	}

	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(port);

	inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));
	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		std::cerr << "Error creating socket\n";
		return 1;
	}
	printf("2\n");
	recv(clientSocket, buffer, sizeof(buffer), 0);
	printf("3\n");
	std::cout << "Received from server: " << buffer << std::endl;
	char *something = readline("Type something for the server: ");
//	send(clientSocket, message, strlen(message), 0);
	send(clientSocket, something, strlen(something), 0);

	close(clientSocket);
	return 0;
}
