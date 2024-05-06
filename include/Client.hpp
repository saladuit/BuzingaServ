#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "CGI.hpp"
#include "FileManager.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "ServerSettings.hpp"
#include "Socket.hpp"

class Client
{
  public:
	Client(const int &server_fd, std::vector<ServerSettings> &serversettings);
	Client() = delete;
	Client(const Client &other) = delete;
	const Client &operator=(const Client &other) = delete;
	~Client();

	ClientState handleConnection(short events);
	void resolveServerSetting();
	int getFD(void) const;

  private:
	HTTPRequest _request;
	HTTPResponse _response;
	FileManager _file_manager;
	CGI _cgi;
	Socket _socket;
	const std::vector<ServerSettings> &_server_list;
	ServerSettings &_serversetting;
	ClientState _state;
};

#endif
