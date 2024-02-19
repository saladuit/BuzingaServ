#include "ClientState.hpp"
#include "ReturnException.hpp"
#include <Client.hpp>
#include <ClientException.hpp>
#include <Logger.hpp>
#include <Server.hpp>
#include <ServerSettings.hpp>

#include <sys/poll.h>

Client::Client(const int &server_fd, const ServerSettings &serversetting)
	: _request(), _file_manager(serversetting), _socket(server_fd),
	  _serversetting(serversetting)
{
	_socket.setupClient();
	_request.setMaxBodySize(_serversetting.getClientMaxBodySize());
}

Client::~Client()
{
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
		// TODO: Fix Error page:
		//  Error status is put in Client::_response
		//  Errorpage fsteam is put into FileManager::_response
		//  in HTTPResponse::send this clashes in the first if statement
		//  FIXED
		return (_state);
	}
	catch (ReturnException &e)
	{
		logger.log(ERROR, "Return exception: " + std::string(e.what()));
		_response.clear();
		_file_manager.setResponse(e.what());
		_file_manager.addToResponse("Location:" + e.getRedirection() +
									"\r\n\r\n");
		_state = ClientState::Sending;
		return (_state);
	}
	return (ClientState::Unkown);
}
