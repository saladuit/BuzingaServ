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
			// implement method Martijn for verifying that we are dealing with a CGI
			// also get fileExtension from Martijn and save in HTTPRequest class
			
			_request.setCGIToTrue(); // settting CGI to true for testing purposes
			// if load then if cgi 

			// _request.setRequestTarget("/python/test.py");
			logger.log(DEBUG, "request target from handleConnection: %", _request.getRequestTarget());
			if (_state == ClientState::Loading && _request.CGITrue() == true)
				_state = _request.parseURIForCGI();
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::CGI_Start)
		{
			logger.log(DEBUG, "Executable of external program is: %", _request.getExecutable());
			_state = _cgi.start(_request.getExecutable(), _request.getEnv(), _request.getBodyLength());
			// _state = ClientState::Done;
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::CGI_Write)
		{
			_state = _cgi.send(_request.getBody(), _request.getBodyLength());
			return (_state);
		}
		else if (events & POLLIN && _state == ClientState::CGI_Read)
		{
			_state = _cgi.receive(_cgi.body);
			logger.log(INFO, "_cgi.body: %", _cgi.body);
			return (_state);
		}
		else if (events & POLLOUT && _state == ClientState::Loading)
		{
			// if (_request._cgi == true)
			// _state = _cgi.createResponse();
			// else 
			// logger.log(INFO, "request target from loading else if: %", _request.getRequestTarget());
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
			logger.log(DEBUG, "request target from handleConnection: %", _request.getRequestTarget());
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
