#include "CGI.hpp"
#include "Poll.hpp"
#include "Client.hpp"
#include "ClientException.hpp"
#include "Logger.hpp"

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/poll.h>

Client::Client(const int &server_fd) : _socket(server_fd)
{
	_socket.setupClient();
	_state = ClientState::Receiving;
	cgiBodyIsSent = false;
	cgiHasBeenRead = false;
}

Client::~Client()
{
}

int Client::getFD(void) const
{
	return (_socket.getFD());
}

int	*Client::getCgiToServerFd(void) {
	return (_cgiToServerFd);
}

int	*Client::getServerToCgiFd(void) {
	return (_serverToCgiFd);
}

HTTPRequest	&Client::getRequest(void) {
	return (_request);
}

ClientState Client::handleConnection(short events, Poll &poll, Client &client, 
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes)
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "Handling client connection on fd: " +
						 std::to_string(_socket.getFD()));
	try
	{
		if (events & POLLIN && _state == ClientState::Receiving)
		{
			logger.log(INFO, "ClientState::Receiving");
			_state = _request.receive(_socket.getFD());
			_request.setCGIToTrue();
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::CGI_Start)
		{
			logger.log(INFO, "ClientState::CGI_Start");
			_state = _cgi.start(poll, client, _request.getBodyLength(), active_pipes);
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::CGI_Write)
		{
			logger.log(INFO, "ClientState::CGI_Write");
			_state = _cgi.send(client, _request.getBody(), _request.getBodyLength());
			return (_state);
		}
		else if (events & POLLIN && _state == ClientState::CGI_Read)
		{
			logger.log(INFO, "ClientState::CGI_Read");
			_state = _cgi.receive(client);
			if (client.cgiHasBeenRead == true) {
				_state = _file_manager.manageCgi(_request.getHTTPVersion(), _cgi.body);
				logger.log(DEBUG, "response:\n\n" + _file_manager.getResponse());
			}
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Loading)
		{
			logger.log(INFO, "ClientState::Loading");
			if (_request.CGITrue() == true) {
				_state = _cgi.parseURIForCGI(_request.getRequestTarget());
				logger.log(DEBUG, "executable: " + _cgi.getExecutable());
				return (_state);
			}
			_state = _file_manager.manage(
				_request.getMethodType(),
				"./data/www" + _request.getRequestTarget(),
				_request.getBody()); // TODO: resolve location
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Error)
		{
			_state = _file_manager.loadErrorPage();
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Sending)
		{
			logger.log(INFO, "ClientState::Sending");
			_state =
				_response.send(_socket.getFD(), _file_manager.getResponse());
			return (_state);
		}
	}
	catch (ClientException &e)
	{
		logger.log(ERROR, "Client exception: " + std::string(e.what()));
		_response.clear();
		_response.append(e.what());
		_state = _file_manager.openErrorPage(
			"./data/errors", e.getStatusCode()); // TODO: resolve location
		return (_state);
	}
	return (ClientState::Unkown);
}
