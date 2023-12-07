#ifndef CGI_HPP
#define CGI_HPP

#include <ClientState.hpp>
#include <HTTPRequest.hpp>
#include <string>
#include <memory>
#include <unordered_map>

#define READ_END 0
#define WRITE_END 1

class Client;
class Poll;

class CGI
{
private:
	pid_t		_pid;
	size_t		_bodyBytesWritten;
	std::string	_executable;
	std::string	_pathInfo;
	std::string	_subPathInfo;
	std::string	_queryString;

  public:
	CGI();
	CGI(const CGI &src) = delete;
	CGI &operator=(const CGI &rhs) = delete;
	~CGI();

	ClientState	start(Poll &poll, Client &client, size_t body_length, 
		std::unordered_map<int, std::shared_ptr<int>> &active_pipes);
	ClientState	parseURIForCGI(std::string requestTarget);
	void		execute(std::string executable);
	bool		fileExists(const std::string& filePath);
	bool		isExecutable(const std::string& filePath);

	const std::string&	getExecutable(void) const;
	void				setExecutable(std::string executable);
	const pid_t&		getPid(void) const;

	ClientState	send(Client &client ,std::string body, size_t bodyLength);
	ClientState	receive(Client &client);

	std::string	body;
	int			pipe_fd[2];
};

#endif
