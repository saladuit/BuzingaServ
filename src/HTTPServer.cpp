#include <HTTPServer.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>
#include <algorithm>
#include <cstring>
#include <memory>
#include <unistd.h>

HTTPServer::~HTTPServer()
{
}

HTTPServer::HTTPServer(const std::string &config_file_path)
	: _parser(config_file_path), _poll()
{
	Logger &logger = Logger::getInstance();
	try
	{
		logger.log(INFO, "Setting up server sockets");
		const std::vector<ServerBlock> &server_blocks =
			_parser.getServerBlocks();
		for (const auto &server_block : server_blocks)
		{
			std::shared_ptr<Server> server =
				std::make_shared<Server>(server_block);
			_active_servers.emplace(server->getFD(), server);
			_poll.addFD(server->getFD(), POLLIN);
		}
	}
	catch (const std::exception &e)
	{
		logger.log(FATAL, e.what());
		exit(EXIT_FAILURE);
	}
}

int HTTPServer::run()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Server started");
	while (true)
	{
		_poll.pollFDs();
		for (auto &poll_fd : _poll.getFds())
		{
			if (poll_fd.revents == 0)
				continue;
			logger.log(DEBUG, "poll fd: " + std::to_string(poll_fd.fd) +
								  " revents: " +
								  _poll.pollEventsToString(poll_fd.revents));
			if (_active_servers.find(poll_fd.fd) != _active_servers.end())
			{
				std::shared_ptr<Client> client =
					std::make_shared<Client>(poll_fd.fd);
				_active_clients.emplace(client->getFD(), client);
				_poll.addFD(client->getFD(), POLLIN);
			}
			else
			{
				_active_clients.at(poll_fd.fd)->handleConnection(poll_fd);
				_poll.removeFD(poll_fd.fd);
				_poll.setEvents(poll_fd.fd, POLLIN);
			}
		}
	}
	return (EXIT_SUCCESS);
}
