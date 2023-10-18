#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <Client.hpp>
#include <ConfigParser.hpp>
#include <Defines.hpp>
#include <Poll.hpp>
#include <Server.hpp>

#include <arpa/inet.h>
#include <poll.h>

#include <memory>
#include <unordered_map>

class HTTPServer
{
  public:
	HTTPServer(const std::string &config_file_path);
	HTTPServer() = delete;
	HTTPServer(const HTTPServer &src) = delete;
	HTTPServer &operator=(const HTTPServer &rhs) = delete;
	~HTTPServer();

	int setup(void);
	int run(void);

  private:
	ConfigParser _parser;
	Poll _poll;
	std::unordered_map<int, std::shared_ptr<Server>> _active_servers;
	std::unordered_map<int, std::shared_ptr<Client>> _active_clients;
};

#endif

/* ************************************************************************** */
