#include "ClientException.hpp"
#include "ClientState.hpp"
#include "HTTPRequest.hpp"
#include "HTTPStatus.hpp"
#include "StatusCode.hpp"
#include <HTTPServer.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>

HTTPServer::HTTPServer(const std::string &config_file_path)
try : _parser(config_file_path), _poll(), _active_servers(), _active_clients(),
	_active_pipes()
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

Client &HTTPServer::getClientByPipeFd(int pipe_fd)
{
	for (const auto &entry : _active_clients)
	{
		const auto &client = entry.second;
		if (client->getCgiToServerFd()[READ_END] == pipe_fd ||
			client->getServerToCgiFd()[WRITE_END] == pipe_fd)
		{
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
		while (true)
			handleActivePollFDs();
	}
	catch (const std::runtime_error &e)
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
	std::vector<std::vector<ServerSettings>> server_list =
		_parser.sortServerSettings();

	for (const std::vector<ServerSettings> &list : server_list)
	{
		std::shared_ptr<Server> server = std::make_shared<Server>(list);
		_active_servers.emplace(server->getFD(), server);
		_poll.addPollFD(server->getFD(), POLLIN);
	}
}

void HTTPServer::handleActivePollFDs()
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "HTTPServer::handleActivePollFDs");

	if (!_poll.pollFDs())
	{
		logger.log(DEBUG, "HTTPServer::Clearup ClientFD's");
		for (auto &pair : _active_clients)
		{
			_poll.setEvents(pair.second->getFD(), POLLOUT);
			pair.second->getResponse().clear();
			HTTPStatus status(StatusCode::RequestTimeout);
			pair.second->getFileManager().setResponse(
				status.getStatusLine("HTTP/1.1") + status.getHTMLStatus());
			pair.second->setState(ClientState::Sending);
		}
	}

	for (const auto &poll_fd : _poll.getPollFDs())
	{
		if (poll_fd.revents == 0)
			continue;

		logger.log(DEBUG, "poll fd: " + std::to_string(poll_fd.fd) +
							  " revents: " +
							  _poll.pollEventsToString(poll_fd.revents));
		try
		{
			if (poll_fd.revents & POLLHUP)
			{
				Client &client = getClientByPipeFd(poll_fd.fd);
				if (client.getRequest().CGITrue() &&
					client.getRequest().getMethodType() != HTTPMethod::DELETE)
				{
					_poll.removeFD(poll_fd.fd);
					_active_pipes.erase(poll_fd.fd);
					_poll.setEvents(client.getFD(), POLLOUT);
					client.setState(ClientState::Sending);
					client.KO = true;
					handleExistingConnection(poll_fd, _poll, client,
											 _active_pipes);
					continue;
				}
			}
			_poll.checkREvents(poll_fd.revents);
		}
		catch (const Poll::PollException &e)
		{
			logger.log(ERROR, e.what());
			_poll.removeFD(poll_fd.fd);
			_active_clients.erase(poll_fd.fd);
			continue;
		}
		if (_active_servers.find(poll_fd.fd) != _active_servers.end())
			handleNewConnection(
				poll_fd.fd,
				_active_servers.find(poll_fd.fd)->second->getServerSettings());
		else if (_active_clients.find(poll_fd.fd) != _active_clients.end())
		{
			Client &client = findClientByFd(poll_fd.fd);
			handleExistingConnection(poll_fd, _poll, client, _active_pipes);
		}
		else if (_active_pipes.find(poll_fd.fd) != _active_pipes.end())
		{
			Client &client = getClientByPipeFd(poll_fd.fd);
			handlePipeConnection(poll_fd, _poll, client, _active_pipes);
		}
		else
			throw std::runtime_error("Unknown file descriptor");
	}
}

void HTTPServer::handlePipeConnection(
	const pollfd &poll_fd, Poll &poll, Client &client,
	std::unordered_map<int, std::shared_ptr<int>> &active_pipes)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "Client % found on poll_fd.fd (pipe): %", &client,
			   poll_fd.fd);

	(&client)->handleConnection(poll_fd.events, poll, client, active_pipes);
	if (client.cgiBodyIsSent)
	{
		logger.log(DEBUG, "remove pipe fd: %", poll_fd.fd);
		_poll.removeFD(poll_fd.fd);
		active_pipes.erase(poll_fd.fd);
		_poll.setEvents(client.getFD(), POLLIN);
	}
	if (client.cgiHasBeenRead)
	{
		logger.log(DEBUG, "remove pipe fd: %", poll_fd.fd);
		_poll.removeFD(poll_fd.fd);
		active_pipes.erase(poll_fd.fd);
		_poll.setEvents(client.getFD(), POLLOUT);
	}
}

void HTTPServer::handleNewConnection(
	int fd, std::vector<ServerSettings> &ServerSettings)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "HTTPServer::handleNewConnection");

	std::shared_ptr<Client> client =
		std::make_shared<Client>(fd, ServerSettings);
	_active_clients.emplace(client->getFD(), client);
	_poll.addPollFD(client->getFD(), POLLIN);
}

void HTTPServer::handleExistingConnection(
	const pollfd &poll_fd, Poll &poll, Client &client,
	std::unordered_map<int, std::shared_ptr<int>> &active_pipes)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "HTTPServer::handleExistingConnection");

	switch (
		(&client)->handleConnection(poll_fd.events, poll, client, active_pipes))
	{
	case ClientState::Receiving:
	case ClientState::CGI_Read:
		_poll.setEvents(poll_fd.fd, POLLIN);
		break;
	case ClientState::Loading:
	case ClientState::Sending:
	case ClientState::Error:
	case ClientState::CGI_Start:
	case ClientState::CGI_Write:
		_poll.setEvents(poll_fd.fd, POLLOUT);
		break;
	case ClientState::Unknown:
	case ClientState::Done:
		_poll.removeFD(poll_fd.fd);
		_active_clients.erase(poll_fd.fd);
		break;
	default:
		throw std::runtime_error(
			"Unknown client state"); // TODO custom exception
	}
}
