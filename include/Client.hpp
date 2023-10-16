#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <CGI.hpp>
#include <FileManager.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <Logger.hpp>

#include <sys/poll.h>

class Client
{
  public:
	Client(pollfd &poll_fd);

	Client() = delete;
	Client(const Client &other) = delete;
	const Client &operator=(const Client &other) = delete;
	~Client();

	void handleConnection(pollfd &poll_fd);

  private:
	HTTPRequest _request;
	HTTPResponse _response;
	FileManager _file_manager;
	CGI _cgi;
	const int _fd;
};

#endif
