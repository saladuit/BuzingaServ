#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>

int accept_new_connection(int serverSocket)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientSocket == -1)
	{
		std::cerr << "Error accepting connection\n";
		return -1;
	}

	char clientIp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIp, INET_ADDRSTRLEN);

	std::cout << "New connection from: " << clientIp << ":" << ntohs(clientAddr.sin_port) << std::endl;

	return clientSocket;
}

// 1) Parsing HTTP Request -- in handleClient we need to parse the HTTP
// request received from the client (GET, POST etc.). I started with this in
// simple_server.cpp
void handleClient(int clientSocket)
{
	const char *message = "Hello from my first simple server!\n";
	char buffer[1024] = {0};

	send(clientSocket, message, strlen(message), 0);

	recv(clientSocket, buffer, sizeof(buffer), 0);

	std::cout << "Received from client: " << buffer << std::endl;
	close(clientSocket);
}

// 2) Processing the Request -- based on the parsed HTTP request we need to
// determine what action the server should take. For example:
// GET -> respond with the appropriate data or file
// POST -> the server may need to process form data

// 3) Sending HTTP Response -- after processing the request we need to generate
// an appropriate HTTP response. This includes setting the:
// - status code
// - headers
// - any data or files that need to be sent back to the client
int main()
{
	int serverSocket = 0;
	int clientSocket = 0;

	struct sockaddr_in serverAddr;
	//	struct sockaddr_in clientAddr;

	int port = 7070;
	int max_socket_so_far = 0;

	fd_set	current_sockets;
	fd_set	ready_sockets;

	FD_ZERO(&current_sockets);
	FD_SET(serverSocket, &current_sockets);
	max_socket_so_far = serverSocket;

	//	socklen_t clientAddrLen = sizeof(clientAddr);

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "Error creating socket\n";
		return EXIT_FAILURE;
	}

	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(port); // Choose any available port

	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (struct sockaddr *)&serverAddr,
			 sizeof(serverAddr)) == -1)
	{
		std::cerr << "Error binding to port\n";
		close(serverSocket);
		return EXIT_FAILURE;
	}

	if (listen(serverSocket, 5) == -1)
	{
		std::cerr << "Error listening\n";
		close(serverSocket);
		return EXIT_FAILURE;
	}

	std::cout << "Server listening on port " << std::to_string(port) << "..." << std::endl;

	while (true)
	{
		std::cout << "Waiting for connections...\n" << std::endl;
		//		clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr,
		//							  &clientAddrLen);
		//		if (clientSocket == -1)
		//		{
		//			std::cerr << "Error accepting connection\n";
		//			continue;
		//		}
		ready_sockets = current_sockets;
		if (select(max_socket_so_far + 1, &ready_sockets, NULL, NULL, NULL) == -1)
		{
			perror("select error");
			exit(EXIT_FAILURE);
		}
		for (size_t i = 0; i <= (size_t) max_socket_so_far; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if ((int) i == serverSocket)
				{
					int clientSocket = accept_new_connection(serverSocket);
					FD_SET(clientSocket, &current_sockets);
					if (clientSocket > max_socket_so_far)
						max_socket_so_far = clientSocket;
				}
				else
				{
					handleClient(clientSocket);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}

	close(serverSocket);
	return (EXIT_SUCCESS);
}
