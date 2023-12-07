#include <HTTPServer.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>

HTTPServer::HTTPServer(const std::string &config_file_path)
try : _parser(config_file_path), _poll(), _active_servers(), _active_clients(), _active_pipes()
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
    throw std::runtime_error("Error: findClientByFd");
}

Client &HTTPServer::getClientByPipeFd(int pipe_fd) {
    for (const auto& entry : _active_clients) {
        const auto& client = entry.second;
        if (client->getCgiToServerFd()[READ_END] == pipe_fd) {
            return *client;
        }
    }
	throw std::runtime_error("Error: getClientByPipeFd");
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
			logger.log(DEBUG, "_active_clients.find(poll_fd.fd) != _active_clients.end()");
			Client &client = findClientByFd(poll_fd.fd);
			handleExistingConnection(poll_fd, _poll, client, _active_pipes);
		}
		else if (_active_pipes.find(poll_fd.fd) != _active_pipes.end()) {
			logger.log(DEBUG, "_active_pipes.find(poll_fd.fd) != _active_pipes.end()");
			logger.log(DEBUG, "poll_fd.fd: %", poll_fd.fd);
			Client &client = getClientByPipeFd(poll_fd.fd);
			logger.log(DEBUG, "Client % found on poll_fd.fd (pipe): %", &client, poll_fd.fd);

			// if (client.getRequest().getMethodType() == HTTPMethod::POST)
			// 	handleExistingConnection(poll_fd, _poll, client, _active_pipes);

			(&client)->handleConnection(poll_fd.events, _poll, client, _active_pipes);
			if (client.cgiHasBeenRead)
			{
				logger.log(DEBUG, "remove pipe fd: %", poll_fd.fd);	
				_poll.removeFD(poll_fd.fd);
				_active_pipes.erase(poll_fd.fd);
				_poll.setEvents(client.getFD(), POLLOUT);
			}
		}
		else
			throw std::runtime_error("Unknown file descriptor");
	}
	logger.log(DEBUG, "HTTPServer::handleActivePollFDs -- after for loop");
}

// void HTTPServer::handleNewPipe(int fd, int pipeState)
// {
// 	Logger &logger = Logger::getInstance();
// 	logger.log(DEBUG, "HTTPServer::handleNewPipe");

// 	std::shared_ptr<int> pipe = std::make_shared<int>(fd);
// 	_active_pipes.emplace(fd, pipe);
// 	_poll.addPollFD(fd, pipeState);
// }

void HTTPServer::handleNewConnection(int fd)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "HTTPServer::handleNewConnection");

	std::shared_ptr<Client> client = std::make_shared<Client>(fd);
	_active_clients.emplace(client->getFD(), client);
	_poll.addPollFD(client->getFD(), POLLIN);
}

void HTTPServer::handleExistingConnection(const pollfd &poll_fd, Poll &poll, Client &client, 
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "HTTPServer::handleExistingConnection");

	// logger.log(DEBUG, "_active_clients.at(poll_fd.fd): %", _active_clients.at(poll_fd.fd));
	logger.log(DEBUG, "client: %", &client);


	switch ((&client)->handleConnection(poll_fd.events, poll, client, active_pipes))
	{
		case ClientState::Receiving:
		case ClientState::CGI_Read:
			_poll.setEvents(poll_fd.fd, POLLIN);
			break;
		// case ClientState::CGI_Read:
		// 	logger.log(DEBUG, "client.getFD: %", client.getFD());
		// 	logger.log(DEBUG, "poll_fd %", poll_fd.fd);
		// 	logger.log(DEBUG, "client.getCgiToServerFd()[READ_END] %", client.getCgiToServerFd()[READ_END]);
		// 	if (poll_fd.fd != client.getCgiToServerFd()[READ_END]) {
		// 		// _poll.setEvents(client.getCgiToServerFd()[READ_END], POLLIN);
		// 		_poll.setEvents(poll_fd.fd, POLLIN);
		// 		break; }
		case ClientState::CGI_Write:
			// _poll.setEvents(client.getServerToCgiFd()[WRITE_END], POLLOUT);
			// break;
		case ClientState::Loading:
		case ClientState::Sending:
		case ClientState::Error:
		case ClientState::CGI_Start:
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
