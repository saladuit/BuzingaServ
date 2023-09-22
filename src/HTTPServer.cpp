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
	int option = 1;

	_server.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server.fd == G_ERROR)
	{
		logger.log(FATAL, strerror(errno));
		throw std::runtime_error("socket creation failed");
	}
	if (setsockopt(_server.fd, SOL_SOCKET, SO_REUSEADDR, &option,
				   sizeof(option)) == G_ERROR)
	{
		logger.log(FATAL, strerror(errno));
		throw std::runtime_error("setsockopt failed");
	}
	bzero(&_server.addr, sizeof(_server.addr));
	_server.addr.sin_family = AF_INET;
	_server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_server.addr.sin_port = htons(18000);
	_server.addr_len = sizeof(_server.addr);
	std::fill_n(_server.addr.sin_zero, sizeof(_server.addr.sin_zero), '\0');
	if (bind(_server.fd, (t_sockaddr *)&_server.addr, _server.addr_len) ==
		G_ERROR)
	{
		logger.log(FATAL, "bind: %", strerror(errno));
		throw std::runtime_error("bind failed");
	}
	if (listen(_server.fd, MAX_PENDING_CONNECTIONS) == G_ERROR)
	{
		logger.log(FATAL, strerror(errno));
		throw std::runtime_error("listen failed");
	}
	_fds.push_back(pollfd{_server.fd, POLLIN, 0});
}

HTTPServer::HTTPServer(const std::string &config_file_path)
	: _parser(config_file_path),
	  _thread_pool(
		  std::stoul(_parser.getGlobalSettings(GlobalSetting::Threads)))
{
	Logger &logger = Logger::getInstance();
	try
	{
		logger.log(INFO, "Setting up server sockets");
		setupServerSocket();
		_thread_pool.Start();
	}
	catch (const std::exception &e)
	{
		logger.log(FATAL, e.what());
		exit(EXIT_FAILURE);
	}
}

void HTTPServer::handleConnection()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Handling connection");
	// size_t bytes_read;
	// int msgsize = 0;
	// std::fill_n(_buffer, sizeof(_buffer), '\0');
	// std::fill_n(_actual_path, sizeof(_actual_path), '\0');
	// do
	// {
	// 	check(bytes_read = read(_client.fd, _buffer + msgsize,
	// 							sizeof(_buffer) - msgsize - 1),
	// 		  "read failed");
	// 	msgsize += bytes_read;
	//
	// 	if (msgsize > BUFFER_SIZE - 1 || _buffer[msgsize - 1] == '\n')
	// 		break;
	// 	std::fill_n(_buffer, sizeof(_buffer), '\0');
	// } while (bytes_read > 0);
	//
	// _buffer[msgsize - 1] = 0;
	// std::cout << "REQUEST: " << _buffer << std::endl;
	// if (realpath(_buffer, _actual_path) == NULL)
	// {
	// 	std::cout << "Error: realpath failed" << std::endl;
	// 	close(_client.fd);
	// 	return;
	// }
	// FILE *file = fopen(_actual_path, "r");
	// if (file == NULL)
	// {
	// 	std::cout << "Error: fopen failed" << std::endl;
	// 	close(_client.fd);
	// 	return;
	// }
	// while (bytes_read = fread(_buffer, 1, sizeof(_buffer), file) > 0)
	// {
	// 	std::cout << "Sending: " << bytes_Read << " bytes" << std::endl;
	// 	write(_client.fd, _buffer, bytes_read);
	// }
	// std::fill_n(_buffer, sizeof(_buffer), '\0');
	// snprintf(_buffer, sizeof(_buffer), "HTTP/1.1 200 OK\r\n\r\nHello");
	// write(_client.fd, _buffer, strlen(_buffer));
	// close(_client.fd);
}

void HTTPServer::acceptConnection()
{
	Logger &logger = Logger::getInstance();
	char client_address[INET_ADDRSTRLEN];

	_client.addr_len = sizeof(_client.addr);
	_client.fd = accept(_server.fd, (t_sockaddr *)&_client.addr,
						(socklen_t *)&_client.addr_len);
	if (_client.fd == G_ERROR)
	{
		logger.log(ERROR, strerror(errno));
		return;
	}
	inet_ntop(AF_INET, &_client.addr.sin_addr, client_address, sizeof(_buffer));
	logger.log(INFO, "Connection received from " + std::string(client_address));
}

int HTTPServer::run()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Server started");
	while (true)
	{
		logger.log(INFO, "Polling for connections");
		logger.log(DEBUG, "Polling % file descriptors", _fds.size());
		int poll_count = poll(_fds.data(), _fds.size(), -1);
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
			{
				logger.log(WARNING, "revents == 0");
				continue;
			}
			if (fd.revents != POLLIN)
			{
				logger.log(WARNING, "revents != POLLIN");
				continue;
			}
			if (fd.fd == _server.fd)
			{
				logger.log(INFO,
						   "Accepting connection: " + std::to_string(fd.fd));
				_thread_pool.QueueTask([&]() { acceptConnection(); });
				_fds.push_back(pollfd{_client.fd, POLLIN, 0});
			}
			else
			{
				logger.log(INFO,
						   "Handling connection: " + std::to_string(fd.fd));
				_thread_pool.QueueTask([&]() { handleConnection(); });
				close(fd.fd);
				fd.fd = -1;
			}
		}
	}
	return (EXIT_SUCCESS);
}
