#include "CGI.hpp"
#include "ClientState.hpp"
#include "LocationSettings.hpp"
#include "Poll.hpp"
#include "ReturnException.hpp"
#include "StatusCode.hpp"
#include <Client.hpp>
#include <ClientException.hpp>
#include <Logger.hpp>
#include <Server.hpp>
#include <ServerSettings.hpp>

#include <stdexcept>
#include <sys/poll.h>
#include <sys/stat.h>
#include <sys/wait.h>

Client::Client(const int &server_fd, std::vector<ServerSettings> &serversetting)
	: _request(), _file_manager(), _socket(server_fd),
	  _server_list(serversetting), _serversetting(serversetting.at(0))
{
	_socket.setupClient();
	_state = ClientState::Receiving;
	cgiBodyIsSent = false;
	cgiHasBeenRead = false;
	KO = false;
}

Client::~Client()
{
}

void Client::resolveServerSetting()
{
	Logger &logger = Logger::getInstance();
	std::string hp;
	try
	{
		hp = _request.getHeader("Host");
	}
	catch (std::out_of_range &e)
	{
		return;
	}

	std::string host = hp.substr(0, hp.find_first_of(":"));
	for (const ServerSettings &block : _server_list)
	{
		std::stringstream ss(block.getServerName());
		std::string name;
		bool flag = false;

		for (; std::getline(ss, name, ' ');)
		{
			logger.log(DEBUG, "resolveServerSetting: compare: " + host + " ? " +
								  name + " " + block.getServerName());
			if (host == name)
			{
				_serversetting = block;
				flag = true;
				break;
			}
		}
		if (flag == true)
			break;
	}
	logger.log(INFO, "resolveServerSetting: Found: " +
						 _serversetting.getServerName());
	_request.setHeaderEnd(false);
	_request.setMaxBodySize(_serversetting.getClientMaxBodySize());
	_file_manager.setServerSetting(_serversetting);
}

int Client::getFD(void) const
{
	return (_socket.getFD());
}

int *Client::getCgiToServerFd(void)
{
	return (_cgiToServerFd);
}

int *Client::getServerToCgiFd(void)
{
	return (_serverToCgiFd);
}

HTTPRequest &Client::getRequest(void)
{
	return (_request);
}

void Client::setState(ClientState state)
{
	_state = state;
}

FileManager &Client::getFileManager()
{
	return (_file_manager);
}

HTTPResponse &Client::getResponse()
{
	return (_response);
}

ClientState Client::handleConnection(
	short events, Poll &poll, Client &client,
	std::unordered_map<int, std::shared_ptr<int>> &active_pipes)
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "Handling client connection on fd: " +
						 std::to_string(_socket.getFD()));
	try
	{
		if (events & POLLIN && _state == ClientState::Receiving)
		{
			logger.log(DEBUG, "ClientState::Receiving");
			_state = _request.receive(_socket.getFD());
			if (_request.getHeaderEnd())
			{
				resolveServerSetting();
				const LocationSettings &loc =
					_serversetting.resolveLocation(_request.getRequestTarget());

				if (loc.resolveMethod(_request.getMethodType()) == false)
					throw ClientException(StatusCode::MethodNotAllowed);
				if (loc.getCGI() == true)
					_request.setCGI(true);
				if (_request.getBody().size() > _request.getMaxBodySize())
					throw ClientException(StatusCode::RequestBodyTooLarge);
				if (_request.getBody().size() >= _request.getBodyLength())
					return (ClientState::Loading);
			}
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::CGI_Start)
		{
			logger.log(DEBUG, "ClientState::CGI_Start");
			_state = _cgi.start(poll, client, _request.getBodyLength(),
								active_pipes);
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::CGI_Write)
		{
			logger.log(DEBUG, "ClientState::CGI_Write");
			_state =
				_cgi.send(client, _request.getBody(), _request.getBodyLength());
			return (_state);
		}
		else if (events & POLLIN && _state == ClientState::CGI_Read)
		{
			logger.log(DEBUG, "ClientState::CGI_Read");
			_state = _cgi.receive(client);
			if (client.cgiHasBeenRead == true)
			{
				_state = _file_manager.manageCgi(_request.getHTTPVersion(),
												 _cgi.body);
				logger.log(DEBUG,
						   "response:\n\n" + _file_manager.getResponse());
			}
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Loading)
		{
			logger.log(DEBUG, "ClientState::Loading");
			if (_request.getCGI() == true &&
				_request.getMethodType() != HTTPMethod::DELETE)
			{
				_state = _cgi.parseURIForCGI(
					_serversetting.resolveLocation(_request.getRequestTarget())
						.resolveAlias(_request.getRequestTarget()));
				logger.log(DEBUG, "executable: " + _cgi.getExecutable());
				return (_state);
			}
			_state = _file_manager.manage(_request.getMethodType(),
										  _request.getRequestTarget(),
										  _request.getBody());
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Error)
		{
			logger.log(DEBUG, "ClientState::Error");
			_state = _file_manager.loadErrorPage();
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Sending)
		{
			logger.log(DEBUG, "ClientState::Sending");
			if (KO == true)
			{
				_state =
					_response.send(_socket.getFD(), "HTTP/1.1 500 KO\t\n\t\n");
				return (_state);
			}
			_state =
				_response.send(_socket.getFD(), _file_manager.getResponse());
			return (_state);
		}
		else
		{
			throw ClientException(StatusCode::BadRequest);
		}
	}
	catch (ClientException &e)
	{
		logger.log(ERROR, "Client exception: " + std::string(e.what()));
		if (_request.getCGI() == true &&
			_request.getMethodType() != HTTPMethod::DELETE)
			_exit(1);
		_response.clear();
		_file_manager.setResponse(e.what());

		const std::string path = _serversetting.getErrorDir();
		if (path.empty())
			_state = _file_manager.openErrorPage("", e.getStatusCode());
		else
			_state =
				_file_manager.openErrorPage(path.substr(1), e.getStatusCode());
		return (_state);
	}
	catch (ReturnException &e)
	{
		logger.log(ERROR, "Return exception: " + std::string(e.what()));
		_response.clear();
		_file_manager.setResponse(e.what());
		_file_manager.addToResponse("Location: " + e.getRedirection() +
									"\r\n\r\n");
		_state = ClientState::Sending;
		return (_state);
	}
	logger.log(DEBUG, "ClientState::UNKNOWN");
	return (ClientState::Unknown);
}
