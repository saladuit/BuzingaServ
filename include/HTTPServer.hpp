#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#define NO_TIMEOUT -1

#include <ConfigParser.hpp>
#include <Defines.hpp>
// #include <ThreadPool.hpp>
#include <HTTPRequest.hpp>

#include <arpa/inet.h>
#include <poll.h>
#include <unordered_map>

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

	bool	is_print(char c);
	int		get_content_length(std::string search_string);

	ConfigParser _parser;
	// ThreadPool _thread_pool;
	std::vector<pollfd> _fds;
	std::vector<int> _server_fds;
	std::unordered_map<int, HTTPRequest> _client_request;

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