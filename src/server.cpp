#include "server.hpp"
#define BACKLOG 5

// functions from The Linux Programming Interface that we're not using yet:
// getaddrinfo()
// setsockopt()
// freeaddrinfo() -- to free the struct allocated by getaddrinfo
// getnameinfo()

static void    get_count_ports(std::ifstream *fd, uint32_t *count_ports)
{
    std::string searchString = "listen";
    std::string line;

    while (std::getline(*fd, line))
	{
        size_t pos = 0;
        while ((pos = line.find(searchString, pos)) != std::string::npos)
		{
            (*count_ports)++;
            pos += searchString.length();
        }
    }
}

static bool	parse_config_file(uint32_t **port, std::string config_file, uint32_t *count_ports)
{
	std::string line;

	std::ifstream fd(config_file);
	if (!fd.is_open())
	{
		std::cerr << "Error opening config file: " << config_file << std::endl;
		return (false);
	}
	get_count_ports(&fd, count_ports);
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

        // htons converts the port address to network byte order with return type uint16_t
		addr[i].sin_port = htons(port[i]); // Choose any available port
		addr[i].sin_addr.s_addr = INADDR_ANY;

		// bind() binds a socket to an address so that clients can locate it
		if (bind(Socket[i], (struct sockaddr *)&addr[i],
				 sizeof(addr[i])) == -1)
		{
			std::cerr << "SERVER: Error binding to port\n";
			close(Socket[i]);
			return;
		}

		// listen() allows a stream socket to accept incoming connections from other sockets
		if (listen(Socket[i], BACKLOG) == -1)
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

int main(int argc, char **argv)
{
	std::vector<int>	sockets;
	uint32_t			count_ports;
	int 				*Socket = NULL;
	struct sockaddr_in	*addr = NULL;
	uint32_t 			*port = NULL;


	if (argc != 1 && argc != 2)
	{
		std::cerr << "SERVER: Invalid number of arguments\n";
		return 1;
	}
	if (argc == 1)
	{
		if (parse_config_file(&port, "conf/default_config_file.conf", &count_ports) == false)
			return (1);
	}
	else
	{
		if (parse_config_file(&port, argv[1], &count_ports) == false)
			return (1);
	}

	set_socket(count_ports, port, Socket, addr, sockets);

	for (size_t i = 0; i < count_ports; i++)
		sockets.push_back(Socket[i]);

	std::vector<struct pollfd> poll_fds(sockets.size());
	for (size_t i = 0; i < sockets.size(); ++i) {
		poll_fds[i].fd = sockets[i];
		poll_fds[i].events = POLLIN;
	}

	while (true)
	{
		int activity = poll(poll_fds.data(), poll_fds.size(), -1);
		if (activity < 0) {
			perror("poll error");
			break;
		}

		for (size_t i = 0; i < sockets.size(); ++i)
		{
			if (poll_fds[i].revents & POLLIN)
			{
				// Handle incoming connection on the respective socket
				// This could involve accepting the connection, receiving data, etc.
				// For brevity, this example just prints a message
				std::cout << "Activity on socket " << sockets[i] << std::endl;
				sleep(1);
				break ;
			}
		}
		break ;
	}

	for (int socket: sockets) {
		close(socket);
	}

	delete[] Socket;
	delete[] addr;
//	system("leaks server");
	return 0;
}