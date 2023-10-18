#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#define NO_TIMEOUT -1

#include <Client.hpp>
#include <ConfigParser.hpp>
#include <Defines.hpp>
#include <Server.hpp>

#include <arpa/inet.h>
#include <poll.h>

#include <memory>
#include <unordered_map>

class HTTPServer
{
  private:
	void logPollfd(const pollfd &fd) const;

	ConfigParser _parser;
	std::vector<pollfd> _poll_fds;
	std::unordered_map<int, std::shared_ptr<Server>> _active_servers;
	std::unordered_map<int, std::shared_ptr<Client>> _active_clients;

  public:
	HTTPServer(const std::string &config_file_path);
	HTTPServer() = delete;
	HTTPServer(const HTTPServer &src) = delete;
	HTTPServer &operator=(const HTTPServer &rhs) = delete;
	~HTTPServer();

	int run(void);
};

#endif
