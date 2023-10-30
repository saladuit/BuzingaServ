#include <HTTPServer.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>

HTTPServer::HTTPServer(const std::string &config_file_path)
try : _parser(config_file_path), _poll(), _active_servers(), _active_clients()
{
}
catch (const std::runtime_error &e)
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
		_parser.ParseConfig();
		setupServers();
		logger.log(INFO, "Server started");
	}
	catch (const std::runtime_error &e)
	{
		logger.log(FATAL, e.what());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);

	while (true)
	{
		try
		{
			handleActivePollFDs();
		}
		catch (const std::runtime_error &e)
		{
			logger.log(FATAL, e.what());
			return (EXIT_FAILURE);
		}
	}
}

void HTTPServer::setupServers(void)
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Setting up server sockets");
	const std::vector<ServerSettings> &server_settings =
		_parser.getServerSettings();

	for (const auto &server_setting : server_settings)
	{
		std::shared_ptr<Server> server =
			std::make_shared<Server>(server_setting);
		_active_servers.emplace(server->getFD(), server);
		_poll.addPollFD(server->getFD(), POLLIN);
	}
}

void HTTPServer::handleActivePollFDs()
{
	Logger &logger = Logger::getInstance();
	_poll.pollFDs();
	for (const auto &poll_fd : _poll.getPollFDs())
	{
		if (poll_fd.revents == 0)
			continue;
		try
		{
			_poll.checkREvents(poll_fd.revents);
		}
		catch (const Poll::PollException &e)
		{
			logger.log(ERROR, e.what());
			_poll.removeFD(poll_fd.fd);
			_active_clients.erase(poll_fd.fd);
			continue;
		}
		logger.log(DEBUG, "poll fd: " + std::to_string(poll_fd.fd) +
							  " revents: " +
							  _poll.pollEventsToString(poll_fd.revents));
		if (_active_servers.find(poll_fd.fd) != _active_servers.end())
			handleNewConnection(poll_fd.fd);
		else if (_active_clients.find(poll_fd.fd) != _active_clients.end())
			handleExistingConnection(poll_fd);
		else
			throw std::runtime_error("Unknown file descriptor");
	}
}

void HTTPServer::handleNewConnection(int fd)
{
	std::shared_ptr<Client> client = std::make_shared<Client>(fd);
	_active_clients.emplace(client->getFD(), client);
	_poll.addPollFD(client->getFD(), POLLIN);
}

void HTTPServer::handleExistingConnection(const pollfd &poll_fd)
{
	switch (_active_clients.at(poll_fd.fd)->handleConnection(poll_fd.events))
	{
	case ClientState::Receiving:
		_poll.setEvents(poll_fd.fd, POLLIN);
		break;
	case ClientState::Loading:
	case ClientState::Sending:
	case ClientState::Error:
		_poll.setEvents(poll_fd.fd, POLLOUT);
		break;
	case ClientState::Done:
		_poll.removeFD(poll_fd.fd);
		_active_clients.erase(poll_fd.fd);
		break;
	default:
		throw std::runtime_error(
			"Unknown client state"); // TODO custom exception
	}
}
