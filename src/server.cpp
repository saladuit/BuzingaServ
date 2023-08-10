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

	if (send(clientSocket, getResponse(indexMethod), strlen(getResponse(indexMethod)), 0) == -1)
		printf("SERVER: send failed\n");
	close(clientSocket);
}

int	main()
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
