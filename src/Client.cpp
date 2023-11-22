#include <Client.hpp>
#include <ClientException.hpp>
#include <Logger.hpp>
#include <sys/stat.h>

#include <sys/poll.h>

Client::Client(const int &server_fd) : _socket(server_fd)
{
	_socket.setupClient();
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
			if (_request.CGITrue() == true)
				_request.parseURIForCGI();
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::start_CGI)
		{
			_cgi.start_CGI(_request.getExecutable(), _request.getEnv(), _request.getBodyLength());
		}
		// else if (events & POLLOUT && _state == ClientState::CGI_Write)
		// {
		// 	_state = _cgi.send(_socket.getFD(), _request.getMethodType(), _request.getBody());
		// 	return (_state);
		// }
		else if (events & POLLIN && _state == ClientState::CGI_Read)
		{
			_state = _cgi.receive(_socket.getFD(), _cgi.body);
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Loading)
		{
			// if (_request._cgi == true)
			// _state = _cgi.createResponse();
			// else 
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
			// if CGI
			// _state =
			//	_response.send(_socket.getFD(), _cgi.getResponse());
			// else
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
