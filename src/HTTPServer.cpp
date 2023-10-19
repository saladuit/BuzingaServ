#include "ServerSetting.hpp"
#include <HTTPServer.hpp>
#include <Logger.hpp>

HTTPServer::HTTPServer(const std::string &config_file_path)
try : _parser(config_file_path), _poll(), _active_servers(), _active_clients()
{
}
catch (const std::exception &e)
{
	Logger &logger = Logger::getInstance();
	logger.log(FATAL, e.what());
	exit(EXIT_FAILURE);
}

HTTPServer::~HTTPServer()
{
}

int HTTPServer::run()
{
	Logger &logger = Logger::getInstance();

	try
	{
		setupServers();
		logger.log(INFO, "Server started");
		while (true)
		{
			_poll.pollFDs();
			handleActivePollFDs();
		}
	}
	catch (const std::exception &e)
	{
		logger.log(FATAL, e.what());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void HTTPServer::setupServers(void)
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "Setting up server sockets");
	const std::vector<ServerSetting> &server_settings =
		_parser.getServerSettings(); // Comented to Compile and test ServerBlock
									 // needs to chang to ServerSetting
	for (const auto &server_block : server_settings)
	{
		std::shared_ptr<Server> server = std::make_shared<Server>(server_block);
		_active_servers.emplace(server->getFD(), server);
		_poll.addFD(server->getFD(), POLLIN);
	}
}

void HTTPServer::handleActivePollFDs()
{
	Logger &logger = Logger::getInstance();
	for (const auto &poll_fd : _poll.getFds())
	{
		if (poll_fd.revents == 0)
			continue;
		logger.log(DEBUG, "poll fd: " + std::to_string(poll_fd.fd) +
							  " revents: " +
							  _poll.pollEventsToString(poll_fd.revents));
		if (_active_servers.find(poll_fd.fd) != _active_servers.end())
		{
			handleNewConnection(poll_fd.fd);
		}
		else if (_active_clients.find(poll_fd.fd) != _active_clients.end())
		{
			_active_clients.at(poll_fd.fd)->handleConnection(poll_fd);
			_poll.removeFD(poll_fd.fd);
		}
		else
			throw std::runtime_error("Unknown file descriptor");
	}
}

void HTTPServer::handleNewConnection(int fd)
{
	std::shared_ptr<Client> client = std::make_shared<Client>(fd);
	_active_clients.emplace(client->getFD(), client);
	_poll.addFD(client->getFD(), POLLIN);
}
