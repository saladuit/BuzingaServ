#include <HTTPServer.hpp>
#include <Logger.hpp>
#include <cstring>
#include <unistd.h>

HTTPServer::~HTTPServer()
{
}

void HTTPServer::setupServerSocket()
{
	Logger &logger = Logger::getInstance();
	t_socket server;
	int option = 1;

	server.fd = socket(AF_INET, SOCK_STREAM, 0);
	logger.log(DEBUG, "Server socket fd: %", server.fd);
	if (server.fd == G_ERROR)
	{
		logger.log(FATAL, strerror(errno));
		throw std::runtime_error("socket creation failed");
	}
	if (setsockopt(server.fd, SOL_SOCKET, SO_REUSEADDR, &option,
				   sizeof(option)) == G_ERROR)
	{
		logger.log(FATAL, strerror(errno));
		throw std::runtime_error("setsockopt failed");
	}
	bzero(&server.addr, sizeof(server.addr));
	server.addr.sin_family = AF_INET;
	server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server.addr.sin_port = htons(18000);
	server.addr_len = sizeof(server.addr);
	std::fill_n(server.addr.sin_zero, sizeof(server.addr.sin_zero), '\0');
	if (bind(server.fd, (t_sockaddr *)&server.addr, server.addr_len) == G_ERROR)
	{
		logger.log(FATAL, "bind: %", strerror(errno));
		throw std::runtime_error("bind failed");
	}
	if (listen(server.fd, MAX_PENDING_CONNECTIONS) == G_ERROR)
	{
		logger.log(FATAL, strerror(errno));
		throw std::runtime_error("listen failed");
	}
	_server_fds.push_back(server.fd);
	_fds.push_back(pollfd{server.fd, POLLIN, 0});
}

HTTPServer::HTTPServer(const std::string &config_file_path)
	: _parser(config_file_path)
// _thread_pool(
//  std::stoul(_parser.getGlobalSettings(GlobalSetting::Threads)))
{
	Logger &logger = Logger::getInstance();
	try
	{
		logger.log(INFO, "Setting up server sockets");
		setupServerSocket();
		// _thread_pool.Start();
	}
	catch (const std::exception &e)
	{
		logger.log(FATAL, e.what());
		exit(EXIT_FAILURE);
	}
}

void HTTPServer::handleConnection(const pollfd &fd)
{
	Logger &logger = Logger::getInstance();
	char _buffer[BUFFER_SIZE];

	logger.log(INFO, "Handling connection");
	read(fd.fd, _buffer, BUFFER_SIZE);
	logger.log(DEBUG, "Received: %", _buffer);
	std::string response = "HTTP/1.1 200 OK\r\n\r\nHello World!";
	write(fd.fd, response.c_str(), response.size());
}

void HTTPServer::acceptConnection(const pollfd &fd)
{
	t_socket client;
	Logger &logger = Logger::getInstance();
	char address[INET_ADDRSTRLEN];

	client.addr_len = sizeof(client.addr);
	client.fd = accept(fd.fd, (t_sockaddr *)&client.addr,
					   (socklen_t *)&client.addr_len);
	if (client.fd == G_ERROR)
	{
		logger.log(ERROR, strerror(errno));
		return;
	}
	inet_ntop(AF_INET, &client.addr.sin_addr, address, sizeof(address));
	logger.log(INFO, "Connection received from " + std::string(address) +
						 " to client fd: " + std::to_string(client.fd));
	_fds.push_back(pollfd{client.fd, POLLIN, 0});
}

void HTTPServer::logPollfd(const pollfd &fd) const
{
	Logger &logger = Logger::getInstance();
	std::stringstream ss;
	ss << "  fd: " << fd.fd
	   << ", revents:" << ((fd.revents & POLLIN) ? " POLLIN" : "")
	   << ((fd.revents & POLLOUT) ? " POLLOUT" : "")
	   << ((fd.revents & POLLHUP) ? " POLLHUP" : "")
	   << ((fd.revents & POLLNVAL) ? " POLLNVAL" : "")
	   << ((fd.revents & POLLPRI) ? " POLLPRI" : "")
	   << ((fd.revents & POLLRDBAND) ? " POLLRDBAND" : "")
	   << ((fd.revents & POLLRDNORM) ? " POLLRDNORM" : "")
	   << ((fd.revents & POLLWRBAND) ? " POLLWRBAND" : "")
	   << ((fd.revents & POLLWRNORM) ? " POLLWRNORM" : "")
	   << ((fd.revents & POLLERR) ? " POLLERR" : "") << std::endl;
	logger.log(DEBUG, ss.str());
}

int HTTPServer::run()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Server started");
	while (true)
	{
		logger.log(INFO, "Polling % file descriptors", _fds.size());
		int poll_count = poll(_fds.data(), _fds.size(), NO_TIMEOUT);
		if (poll_count == G_ERROR)
		{
			logger.log(ERROR, strerror(errno));
			continue;
		}
		if (poll_count == 0)
		{
			logger.log(WARNING, "poll() timed out");
			continue;
		}
		for (auto &fd : _fds)
		{
			if (fd.revents == 0)
				continue;
			logPollfd(fd);
			if (std::find(_server_fds.begin(), _server_fds.end(), fd.fd) !=
				_server_fds.end())
			{
				logger.log(INFO, "Accepting connection on server fd: " +
									 std::to_string(fd.fd));
				acceptConnection(fd);
				// _thread_pool.QueueTask([&]() { acceptConnection(); });
			}
			else
			{
				logger.log(INFO, "Handling connection of client fd: " +
									 std::to_string(fd.fd));
				handleConnection(fd);
				// _thread_pool.QueueTask([&]() { handleConnection(); });
			}
		}
	}
	return (EXIT_SUCCESS);
}
