#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <CGI.hpp>
#include <FileManager.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <Socket.hpp>
#include <memory>
#include <unordered_map>

class Client
{
  public:
	Client(const int &server_fd);
	Client() = delete;
	Client(const Client &other) = delete;
	const Client &operator=(const Client &other) = delete;
	~Client();

	ClientState handleConnection(short events, Poll &poll, Client &client, 
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes);
	int getFD(void) const;
	int	*getCgiToServerFd(void);
	int	*getServerToCgiFd(void);
	HTTPRequest	&getRequest(void);
	void	setState(ClientState state);

	bool	cgiBodyIsSent;
	bool	cgiHasBeenRead;
	bool	KO;

  private:
	HTTPRequest		_request;
	HTTPResponse	_response;
	FileManager		_file_manager;
	CGI				_cgi;
	Socket			_socket;
	ClientState		_state;
	int				_serverToCgiFd[2];
	int				_cgiToServerFd[2];
};

#endif
