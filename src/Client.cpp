#include "ClientState.hpp"
#include "ReturnException.hpp"
#include <Client.hpp>
#include <ClientException.hpp>
#include <Logger.hpp>
#include <Server.hpp>
#include <ServerSettings.hpp>

#include <sys/poll.h>

Client::Client(const int &server_fd, std::vector<ServerSettings> &serversetting)
	: _request(), _file_manager(), _socket(server_fd),
	  _server_list(serversetting), _serversetting(serversetting.at(0))
{
	_socket.setupClient();
}

Client::~Client()
{
}

void Client::resolveServerSetting()
{
	Logger &logger = Logger::getInstance();
	const std::string &hp = _request.getHeader("Host");
	std::string host = hp.substr(0, hp.find_first_of(":"));
	for (const ServerSettings &block : _server_list)
	{
		std::stringstream ss(block.getServerName());
		std::string name;

		for (; std::getline(ss, name, ' ');)
		{
			if (host == name)
			{
				_serversetting = block;
				_request.setHeaderEnd(false);
				break;
			}
		}
		if (_request.getHeaderEnd() == false)
			break;
	}
	logger.log(INFO, "Found ServerSetting: " + _serversetting.getServerName());
	_request.setMaxBodySize(_serversetting.getClientMaxBodySize());
	_file_manager.setServerSetting(_serversetting);
}

int Client::getFD(void) const
{
	return (_socket.getFD());
}

ClientState Client::handleConnection(short events)
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "Handling client connection on fd: " +
						 std::to_string(_socket.getFD()));
	try
	{
		if (events & POLLIN)
		{
			_state = _request.receive(_socket.getFD());
			if (_request.getHeaderEnd())
				resolveServerSetting();
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Loading)
		{
			_state = _file_manager.manage(_request.getMethodType(),
										  _request.getRequestTarget(),
										  _request.getBody());
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Error)
		{
			_state = _file_manager.loadErrorPage();
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Sending)
		{
			_state =
				_response.send(_socket.getFD(), _file_manager.getResponse());
			return (_state);
		}
	}
	catch (ClientException &e)
	{
		logger.log(ERROR, "Client exception: " + std::string(e.what()));
		_response.clear();
		_file_manager.setResponse(e.what());
		_state = _file_manager.openErrorPage(
			_serversetting.getErrorDir().substr(1), e.getStatusCode());
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
	return (ClientState::Unkown);
}
