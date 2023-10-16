#include <HTTPServer.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>
#include <algorithm>
#include <cstring>
#include <unistd.h>

HTTPServer::~HTTPServer()
{
}

HTTPServer::HTTPServer(const std::string &config_file_path)
	: _parser(config_file_path), _poll_fds(0), _active_servers(),
	  _active_clients()
{
	Logger &logger = Logger::getInstance();
	try
	{
		logger.log(INFO, "Setting up server sockets");
		const std::vector<ServerBlock> &server_blocks =
			_parser.getServerBlocks();
		for (const auto &server_block : server_blocks)
		{
			Server server(server_block);
			_active_servers.emplace(server.getFD(), server);
			_poll_fds.push_back(pollfd{server.getFD(), POLLIN, 0});
		}
	}
	catch (const std::exception &e)
	{
		logger.log(FATAL, e.what());
		exit(EXIT_FAILURE);
	}
}

void HTTPServer::logPollfd(const pollfd &fd) const
{
	Logger &logger = Logger::getInstance();
	std::stringstream ss;
	ss << "  fd: " << fd.fd
	   << ", revents:" << ((fd.revents & POLLIN) ? " POLLIN" : "")
	   << ((fd.revents & POLLOUT) ? " POLLOUT" : "")
	   << ((fd.revents & POLLHUP) ? " POLLHUP" : "")
	   << ((fd.revents & POLLNVAL) ? " POLLNVAL" : "")
	   << ((fd.revents & POLLPRI) ? " POLLPRI" : "")
	   << ((fd.revents & POLLRDBAND) ? " POLLRDBAND" : "")
	   << ((fd.revents & POLLRDNORM) ? " POLLRDNORM" : "")
	   << ((fd.revents & POLLWRBAND) ? " POLLWRBAND" : "")
	   << ((fd.revents & POLLWRNORM) ? " POLLWRNORM" : "")
	   << ((fd.revents & POLLERR) ? " POLLERR" : "") << std::endl;
	logger.log(DEBUG, ss.str());
}

int HTTPServer::run()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Server started");
	while (true)
	{
		logger.log(INFO, "Polling % file descriptors", _poll_fds.size());
		int poll_count = poll(_poll_fds.data(), _poll_fds.size(), NO_TIMEOUT);
		if (poll_count == SYSTEM_ERROR)
			throw SystemException("poll");
		if (poll_count == 0)
		{
			logger.log(WARNING, "poll() timed out");
			continue;
		}
		for (auto &pollfd : _poll_fds)
		{
			if (pollfd.revents == 0)
				continue;
			logPollfd(pollfd);
			if (_active_servers.find(pollfd.fd) != _active_servers.end())
			{
				Client client(pollfd.fd);
				_active_clients.emplace(client.getFD(), client);
				_poll_fds.push_back(pollfd{fd, POLLIN, 0});
			}
			else
				_active_clients.at(pollfd.fd).handleConnection(pollfd);
		}
	}
	return (EXIT_SUCCESS);
}
