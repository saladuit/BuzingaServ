#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <CGI.hpp>
#include <FileManager.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <Socket.hpp>

class Client
{
  public:
	Client(const int &server_fd);
	Client() = delete;
	Client(const Client &other) = delete;
	const Client &operator=(const Client &other) = delete;
	~Client();

	ClientState handleConnection(short events);
	int getFD(void) const;

  private:
	HTTPRequest _request;
	HTTPResponse _response;
	FileManager _file_manager;
	CGI _cgi;
	Socket _socket;
	ClientState _state;
};

#endif
