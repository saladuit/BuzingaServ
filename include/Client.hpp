#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "AutoIndexGenerator.hpp"
#include "CGI.hpp"
#include "FileManager.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "ServerSettings.hpp"
#include "Socket.hpp"

#include <memory>
#include <unistd.h>
#include <unordered_map>

class Client
{
  public:
	Client(const int &server_fd, std::vector<ServerSettings> &serversettings);
	Client() = delete;
	Client(const Client &other) = delete;
	const Client &operator=(const Client &other) = delete;
	~Client();

	ClientState handleConnection(
		short events, Poll &poll, Client &client,
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes);
	void resolveServerSetting();
	int getFD(void) const;
	int *getCgiToServerFd(void);
	int *getServerToCgiFd(void);
	HTTPRequest &getRequest(void);
	void setState(ClientState state);

	FileManager &getFileManager();
	HTTPResponse &getResponse();

	bool cgiBodyIsSent;
	bool cgiHasBeenRead;
	bool KO;

  private:
	HTTPRequest _request;
	HTTPResponse _response;
	FileManager _file_manager;
	CGI _cgi;
	Socket _socket;
	const std::vector<ServerSettings> &_server_list;
	ServerSettings _serversetting;
	ClientState _state;
	int _serverToCgiFd[2];
	int _cgiToServerFd[2];

	void checkCGI(const std::string &request_target, HTTPMethod method);
};

#endif
