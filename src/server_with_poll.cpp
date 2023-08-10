#include "server.hpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

void handleClient(int clientSocket)
{
	const char *message = "Hello from my first simple server!\n";
	char buffer[1024] = {0};

	send(clientSocket, message, strlen(message), 0);

	recv(clientSocket, buffer, sizeof(buffer), 0);

	std::cout << "Received from client: " << buffer << std::endl;
	close(clientSocket);
}

int main()
{
	int serverSocket;
	int clientSocket;

	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

	int port = PORT_NUM;

	socklen_t clientAddrLen = sizeof(clientAddr);

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "Error creating socket\n";
		return 1;
	}

	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(port); // Choose any available port

	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (struct sockaddr *)&serverAddr,
			 sizeof(serverAddr)) == -1)
	{
		std::cerr << "Error binding to port\n";
		close(serverSocket);
		return 1;
	}

	if (listen(serverSocket, 5) == -1)
	{
		std::cerr << "Error listening\n";
		close(serverSocket);
		return 1;
	}

	std::cout << "Server listening on port " << std::to_string(port) << "..." << std::endl;

	// Set up an array of pollfd structures to store information about each socket
	const int MAX_SOCKETS = 10; // Adjust this according to your needs
	struct pollfd sockets[MAX_SOCKETS];

	// Initialize the pollfd array
	sockets[0].fd = serverSocket;
	sockets[0].events = POLLIN; // Wait for incoming data
	int numSockets = 1; // Number of sockets being monitored

	while (true) {
		// Call poll to wait for activity on any of the sockets
		int ready = poll(sockets, numSockets, -1); // -1 means no timeout

		if (ready == -1) {
			perror("poll error");
			exit(EXIT_FAILURE);
		}

		// Check if the server socket has incoming connection requests
		if (sockets[0].revents & POLLIN) {
			clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
			if (clientSocket == -1) {
				std::cerr << "Error accepting connection\n";
				continue;
			}

			// Add the new client socket to the pollfd array
			sockets[numSockets].fd = clientSocket;
			sockets[numSockets].events = POLLIN;
			numSockets++;

			std::cout << "New client connected\n";
		}

		// Check for data on client sockets
		for (int i = 1; i < numSockets; i++) {
			printf("a\n");
			if (sockets[i].revents & POLLIN) {
				printf("b\n");
				handleClient(sockets[i].fd);
				printf("c\n");
				// Close the client socket and remove it from the pollfd array
				close(sockets[i].fd);
				sockets[i] = sockets[numSockets - 1];
				numSockets--;
				i--; // Re-check the current index in the next iteration
			}
		}
	}


	close(serverSocket);
	return 0;
}
