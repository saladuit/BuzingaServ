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

Client &HTTPServer::findClientByFd(int targetFd)
{
    auto it = _active_clients.find(targetFd);

    if (it != _active_clients.end())
        return *(it->second);
    throw std::runtime_error("Error: poll_fd.fd refers to a Server instead of a Client");
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
	while (true)
	{
		logger.log(DEBUG, "while loop HTTPServer::run");
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
	return (EXIT_SUCCESS);
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
	logger.log(DEBUG, "HTTPServer::handleActivePollFDs");

	_poll.pollFDs();
	for (const auto &poll_fd : _poll.getPollFDs())
	{
		logger.log(DEBUG, "HTTPServer::handleActivePollFDs -- in for loop");
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
		else if (_active_clients.find(poll_fd.fd) != _active_clients.end()) {
			Client &client = findClientByFd(poll_fd.fd);
			handleExistingConnection(poll_fd, client);
		}
		else
			throw std::runtime_error("Unknown file descriptor");
	}
	logger.log(DEBUG, "HTTPServer::handleActivePollFDs -- after for loop");
}

void HTTPServer::handleNewConnection(int fd)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "HTTPServer::handleNewConnection");

	std::shared_ptr<Client> client = std::make_shared<Client>(fd);
	_active_clients.emplace(client->getFD(), client);
	_poll.addPollFD(client->getFD(), POLLIN);
}

void HTTPServer::handleExistingConnection(const pollfd &poll_fd, Client &client)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "HTTPServer::handleExistingConnection");

	switch (_active_clients.at(poll_fd.fd)->handleConnection(poll_fd.events, client))
	{
		case ClientState::Receiving:
			_poll.setEvents(poll_fd.fd, POLLIN);
			break;
		case ClientState::CGI_Read:
			logger.log(DEBUG, "client.getFD: %", client.getFD());
			logger.log(DEBUG, "poll_fd %", poll_fd.fd);
			_poll.setEvents(client.getCgiToServerFd()[READ_END], POLLIN);
			break;
		case ClientState::Loading:
		case ClientState::Sending:
		case ClientState::Error:
		case ClientState::CGI_Write:
		case ClientState::CGI_Start:
		case ClientState::CGI_Load:
			// does it make sense to set GCI_Load POLLOUT here? 
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
