#ifndef CGI_HPP
#define CGI_HPP

#include <ClientState.hpp>
#include <HTTPRequest.hpp>
#include <string>

#define READ_END 0
#define WRITE_END 1

class Client;

// Common gateway interface

class CGI
{
private:
	pid_t		_pid;
	// int			_serverToExternalProgram[2];
	// int			_externalProgramToServer[2];
	bool		_bodyIsSent;
	size_t		_bodyBytesWritten;
	std::string	_executable;

  public:
	CGI();
	CGI(const CGI &src) = delete;
	CGI &operator=(const CGI &rhs) = delete;
	~CGI();

	// execute is probably redundant
	ClientState	start(size_t body_length, Client &client);
	ClientState	parseURIForCGI(std::string requestTarget);
	void		execute(std::string executable, char **env);
	bool		fileExists(const std::string& filePath);
	bool		isExecutable(const std::string& filePath);

	const std::string&	getExecutable(void) const;
	const pid_t&		getPid(void) const;

	ClientState	send(Client &client ,std::string body, size_t bodyLength);
	ClientState	receive(Client &client);

	std::string	body;
	int			pipe_fd[2];
	char		**_env;
};

#endif
