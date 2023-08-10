#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// readline
#include "server.hpp"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

void	validateAndSend(char *method, int clientSocket)
{
	char get[] =
		"GET / HTTP/1.1\nHost: "
		"localhost:8081\nConnection: keep-alive\nCache-Control: max-age=0\n"
		"sec-ch-ua: \"Not/A)Brand\";v=\"99\", \"Brave\";v=\"115\", "
		"\"Chromium\";v=\"115\"\n"
		"sec-ch-ua-mobile: ?0\n"
		"sec-ch-ua-platform: \"macOS\"\n"
		"Upgrade-Insecure-Requests: 1\n"
		"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) "
		"AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 "
		"Safari/537.36\n"
		"Accept: "
		"text/html,application/xhtml+xml,application/xml;q=0.9,image/"
		"avif,image/webp,image/apng,*/*;q=0.8\n"
		"Sec-GPC: 1\n"
		"Accept-Language: en-US,en\n"
		"Sec-Fetch-Site: none\n"
		"Sec-Fetch-Mode: navigate\n"
		"Sec-Fetch-User: ?1\n"
		"Sec-Fetch-Dest: document\n"
		"Accept-Encoding: gzip, deflate, br\n";

	char post[] = "POST / HTTP/1.1\nHost:localhost\nand more\n";

	char del[] = "DELETE / HTTP/1.1\nHost:\nenzo\n";

	if (strncmp(method, "GET", strlen("GET")) == 0) {
		if (send(clientSocket, get, strlen(get), 0) == -1)
			printf("CLIENT: send failed\n");
	}
	else if (strncmp(method, "POST", strlen("POST")) == 0)
	{
		if (send(clientSocket, post, strlen(post), 0) == -1)
			printf("CLIENT: send failed\n");
	}
	else if (strncmp(method, "DELETE", strlen("DELETE")) == 0)
	{
		if (send(clientSocket, del, strlen(del), 0) == -1)
			printf("CLIENT: send failed\n");
	}
	else
	{
		if (send(clientSocket, "bal met jus\n", strlen("bal met jus\n"), 0) == -1)
			printf("CLIENT: send failed\n");
	}
}

int main()
{
	int clientSocket;

	// this fixed buffer size could lead to crashes
	char buffer[1024] = {0};
	struct sockaddr_in serverAddr;
	int port = PORT_NUM;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1)
	{
		std::cerr << "CLIENT: Error creating socket\n";
		return 1;
	}

	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(port);

	inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

	// client opens connection
	if (connect(clientSocket, (struct sockaddr *)&serverAddr,
				sizeof(serverAddr)) == -1)
	{
		std::cerr << "CLIENT: Error creating socket\n";
		return 1;
	}

	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << "Received from server: " << buffer << std::endl;

	char *method = readline("What method do you want to test?\n");

	// send request
	validateAndSend(method, clientSocket);

	char responseBuffer[1024] = {0};

	// receive response
	recv(clientSocket, responseBuffer, sizeof(responseBuffer), 0);
	std::cout << "Received from server: " << responseBuffer << std::endl;
	close(clientSocket);
	return 0;
}
