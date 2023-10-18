#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <CGI.hpp>
#include <FileManager.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <Logger.hpp>
#include <Socket.hpp>

#include <sys/poll.h>

class Client
{
  public:
	Client(const int &server_fd);

	Client() = delete;
	Client(const Client &other);
	const Client &operator=(const Client &other) = delete;
	~Client();

	void handleConnection(const pollfd &poll_fd);
	int getFD(void) const;

  private:
	HTTPRequest _request;
	HTTPResponse _response;
	FileManager _file_manager;
	CGI _cgi;
	Socket _socket;
};

#endif
