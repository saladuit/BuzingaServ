#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <ConfigParser.hpp>
#include <Defines.hpp>
#include <ThreadPool.hpp>

#include <arpa/inet.h>
#include <poll.h>

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
	void setupServerSocket(void);
	void handleConnection(void);
	void acceptConnection(void);

	ConfigParser _parser;
	ThreadPool _thread_pool;
	std::vector<pollfd> _fds;
	const std::string _config_path;
	t_socket _server;
	t_socket _client;
	char _buffer[BUFFER_SIZE];

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
