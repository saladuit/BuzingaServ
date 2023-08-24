#include "server.hpp"

//const char *getResponse(t_method_index methodIndex)
//{
//	const char *getResponse = "GET RESPONSE !!!";
//	const char *postResponse = "POST RESPONSE ???";
//	const char *delResponse = "DELETE RESPONSE >>>>>>>>>";
//	const char *errorResponse = "!@! ERROR !@!";
//	if (methodIndex == GET)
//		return (getResponse);
//	else if (methodIndex == POST)
//		return (postResponse);
//	else if (methodIndex == DELETE)
//		return (delResponse);
//	else
//		return (errorResponse);
//}
//
//t_method_index getMethodIndex(char *request)
//{
//	const char *get = "GET";
//	const char *post = "POST";
//	const char *del = "DELETE";
//
//	if (strncmp(request, get, strlen(get)) == 0)
//		return (GET);
//	else if (strncmp(request, post, strlen(post)) == 0)
//		return (POST);
//	else if (strncmp(request, del, strlen(del)) == 0)
//		return (DELETE);
//	else
//		return (ERROR);
//}
//
//void handleClient(int clientSocket)
//{
//	t_method_index indexMethod;
//	//	char 			*response;
//
//	// it might be necessary to free buffer here
//	const char *message = "Hello from my first simple server!\n";
//	char buffer[1024] = {0};
//
//	if (send(clientSocket, message, strlen(message), 0) == -1)
//		printf("SERVER: send failed\n");
//
//	if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1)
//		printf("SERVER: recv failed\n");
//
//	std::cout << "Received from client:\n" << buffer << std::endl;
//
//	indexMethod = getMethodIndex(buffer);
//
//	if (send(clientSocket, getResponse(indexMethod),
//			 strlen(getResponse(indexMethod)), 0) == -1)
//		printf("SERVER: send failed\n");
//	close(clientSocket);
//}

static bool	parse_config_file(uint32_t **port, std::string config_file, uint32_t *count_ports)
{
	std::ifstream fd(config_file);
	if (!fd.is_open())
	{
		std::cerr << "Error opening config file: " << config_file << std::endl;
		return (false);
	}
    std::string searchString = "listen";
    std::string line;

    while (std::getline(fd, line)) {
        size_t pos = 0;
        while ((pos = line.find(searchString, pos)) != std::string::npos) {
            (*count_ports)++;
            pos += searchString.length();
        }
    }
//    std::cout << std::to_string(count) << std::endl;
    *port = new uint32_t[*count_ports + 1];
	if (!(*port))
	{
		std::cout << "Error on allocating the port" << std::endl;
		return (false);
	}
	uint32_t j = 0;
	std::string portStr;
    fd.clear();
    fd.seekg(0, std::ios::beg);
	while (std::getline(fd, line) && j < *count_ports)
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
			std::cout << "Port number:" << portStr << std::endl;
            if (!portStr.empty()) {
				(*port)[j++] = std::stoul(portStr);
            } else {
                std::cerr << "Port number not found in the configuration." << std::endl;
                return (false);
            }
		}
	}
	fd.close();
	return (true);
}

void set_socket(uint32_t count_ports, uint32_t *port, int *&Socket, sockaddr_in *&addr, std::vector<int> &sockets)
{
	delete[] Socket; // Clean up previous allocation if applicable
	Socket = new int[count_ports + 1];
	if (!Socket)
	{
		std::cout << "Error on allocating the socket" << std::endl;
		return;
	}
	delete[] addr; // Clean up previous allocation if applicable
	addr = new sockaddr_in[count_ports + 1];
	if (!addr)
	{
		std::cout << "Error on allocating the socket" << std::endl;
		delete[] Socket; // Clean up memory allocation
		return;
	}
	sockets.clear(); // Clear the vector before populating it
	for (uint32_t i = 0; i < count_ports; i++)
	{
		Socket[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (Socket[i] == -1)
		{
			std::cerr << "SERVER: Error creating socket\n";
			return;
		}
		addr[i].sin_family = AF_INET;
		addr[i].sin_port = htons(port[i]); // Choose any available port
		addr[i].sin_addr.s_addr = INADDR_ANY;

		if (bind(Socket[i], (struct sockaddr *)&addr[i],
				 sizeof(addr[i])) == -1)
		{
			std::cerr << "SERVER: Error binding to port\n";
			close(Socket[i]);
			return;
		}

		if (listen(Socket[i], 5) == -1)
		{
			std::cerr << "SERVER: Error listening\n";
			close(Socket[i]);
			return;
		}

		std::cout << "Server listening on port " << std::to_string(port[i])
				  << "...\n\n";

		sockets.push_back(Socket[i]);
	}
}

int main(int argc, char **argv) {
//	int serverSocket;
//	int clientSocket;
	std::vector<int> sockets;
	uint32_t count_ports;
	int *Socket = NULL;



//	struct sockaddr_in serverAddr;
	struct sockaddr_in *addr = NULL;
//	struct sockaddr_in clientAddr;

	uint32_t *port = NULL;


	if (argc != 1 && argc != 2) {
		std::cerr << "SERVER: Invalid number of arguments\n";
		return 1;
	}
	if (argc == 1) {
		if (parse_config_file(&port, "conf/default_config_file.conf", &count_ports) == false)
			return (1);
	} else {
		if (parse_config_file(&port, argv[1], &count_ports) == false) {
			return (1);
		}
	}
//	socklen_t clientAddrLen = sizeof(clientAddr);

	set_socket(count_ports, port, Socket, addr, sockets);

	// ##################### moved to set_socket #########################

//	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//	if (serverSocket == -1)
//	{
//		std::cerr << "SERVER: Error creating socket\n";
//		return 1;
//	}

//	serverAddr.sin_family = AF_INET;
//
//	serverAddr.sin_port = htons(port[1]); // Choose any available port
//	serverAddr.sin_addr.s_addr = INADDR_ANY;

//	if (bind(serverSocket, (struct sockaddr *)&serverAddr,
//			 sizeof(serverAddr)) == -1)
//	{
//		std::cerr << "SERVER: Error binding to port\n";
//		close(serverSocket);
//		return 1;
//	}
//
//	if (listen(serverSocket, 5) == -1)
//	{
//		std::cerr << "SERVER: Error listening\n";
//		close(serverSocket);
//		return 1;
//	}

//	std::cout << "Server listening on port " << std::to_string(port[1])
//			  << "...\n\n";

	// ############################################################################

	// try to do something with the clients message
	// -> send something back for example or an error code if the request cannot
	// be handled

	// ---------------------------------------------------------------------------- old
//	while (true)
//	{
//		clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr,
//							  &clientAddrLen);
//		if (clientSocket == -1)
//		{
//			std::cerr << "SERVER: Error accepting connection\n";
//			continue;
//		}
//		handleClient(clientSocket);
//	}
	// ---------------------------------------------------------------------------- old


	// ******************************************************************************************


//	sockets.push_back(socket1);
//	sockets.push_back(socket2);
//	sockets.push_back(sockets);
	for (size_t i = 0; i < count_ports; i++)
		sockets.push_back(Socket[i]);

	std::vector<struct pollfd> poll_fds(sockets.size());
	for (size_t i = 0; i < sockets.size(); ++i) {
		poll_fds[i].fd = sockets[i];
		poll_fds[i].events = POLLIN;
	}

	while (true) {
		int activity = poll(poll_fds.data(), poll_fds.size(), -1);
		if (activity < 0) {
			perror("poll error");
			break;
		}

		for (size_t i = 0; i < sockets.size(); ++i) {
			if (poll_fds[i].revents & POLLIN) {
				// Handle incoming connection on the respective socket
				// This could involve accepting the connection, receiving data, etc.
				// For brevity, this example just prints a message
				std::cout << "Activity on socket " << sockets[i] << std::endl;
				sleep(1);
			}
		}
	}

	// Close sockets and perform cleanup
	for (int socket: sockets) {
		close(socket);
	}

	// ******************************************************************************************

	delete[] Socket;
	delete[] addr;
	return 0;

}