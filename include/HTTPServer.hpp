#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#define NO_TIMEOUT -1

#include <ConfigParser.hpp>
#include <Defines.hpp>
// #include <ThreadPool.hpp>

#include <arpa/inet.h>
<<<<<<< HEAD
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
=======
#include <poll.h>
>>>>>>> main

typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr t_sockaddr;

typedef struct s_socket
{
	int fd;
	t_sockaddr_in addr;
	socklen_t addr_len;
} t_socket;

class HTTPServer
{
  private:
	void setupServerSocket(const ServerBlock &server_block);
	void handleConnection(pollfd &fd);
	void acceptConnection(const pollfd &fd);
	void logPollfd(const pollfd &fd) const;

	ConfigParser _parser;
	// ThreadPool _thread_pool;
	std::vector<pollfd> _fds;
	std::vector<int> _server_fds;

  public:
	HTTPServer(const std::string &config_file_path);
	HTTPServer() = delete;
	HTTPServer(const HTTPServer &src) = delete;
	HTTPServer &operator=(const HTTPServer &rhs) = delete;
	~HTTPServer();

	int run(void);
};

#endif

/* ************************************************************************** */
