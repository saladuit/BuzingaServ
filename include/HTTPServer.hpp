#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <Client.hpp>
#include <ConfigParser.hpp>
#include <Poll.hpp>
#include <Server.hpp>

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

	int run(void);

  private:
	ConfigParser _parser;
	Poll _poll;
	std::unordered_map<int, std::shared_ptr<Server>> _active_servers;
	std::unordered_map<int, std::shared_ptr<Client>> _active_clients;
	std::unordered_map<int, std::shared_ptr<int>> _active_pipes;

	void setupServers(void);
	void handleActivePollFDs();
	void handleNewConnection(int fd, std::vector<ServerSettings> &ServerBlock);
	void handleExistingConnection(
		const pollfd &poll_fd, Poll &poll, Client &client,
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes);
	Client &findClientByFd(int targetFd);
	Client &getClientByPipeFd(int pipe_fd);
	void handlePipeConnection(
		const pollfd &poll_fd, Poll &poll, Client &client,
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes);
};

#endif
