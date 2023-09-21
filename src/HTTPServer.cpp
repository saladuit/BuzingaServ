#include <HTTPServer.hpp>
#include <Logger.hpp>
#include <cstring>
#include <unistd.h>

HTTPServer::~HTTPServer()
{
}

void HTTPServer::check(int exp, const std::string &msg)
{
	if (exp == G_ERROR)
	{
		Logger &logger = Logger::getInstance();
		logger.log(LogLevel::ERROR, msg);
	}
}

void HTTPServer::setupServerSocket()
{
	check(_server.fd = socket(AF_INET, SOCK_STREAM, 0),
		  "socket creation failed");
	bzero(&_server.addr, sizeof(_server.addr));
	_server.addr.sin_family = AF_INET;
	_server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_server.addr.sin_port = htons(SERVER_PORT);
	_server.addr_len = sizeof(_server.addr);
	std::fill_n(_server.addr.sin_zero, sizeof(_server.addr.sin_zero), '\0');
	check(bind(_server.fd, (t_sockaddr *)&_server.addr, _server.addr_len),
		  "bind failed");
	check(listen(_server.fd, MAX_PENDING_CONNECTIONS), "listen failed");
}

HTTPServer::HTTPServer(const std::string &config_file_path)
	: _parser(config_file_path),
	  _thread_pool(
		  std::stoul(_parser.getGlobalSettings(GlobalSetting::Threads)))
{
	setupServerSocket();
	_thread_pool.Start();
}

void HTTPServer::handleConnection()
{
	size_t bytes_read;
	int msgsize = 0;
	std::fill_n(_buffer, sizeof(_buffer), '\0');
	std::fill_n(_actual_path, sizeof(_actual_path), '\0');
	do
	{
		check(bytes_read = read(_client.fd, _buffer + msgsize,
								sizeof(_buffer) - msgsize - 1),
			  "read failed");
		msgsize += bytes_read;

		if (msgsize > BUFFER_SIZE - 1 || _buffer[msgsize - 1] == '\n')
			break;
		std::fill_n(_buffer, sizeof(_buffer), '\0');
	} while (bytes_read > 0);

	_buffer[msgsize - 1] = 0;
	std::cout << "REQUEST: " << _buffer << std::endl;
	if (realpath(_buffer, _actual_path) == NULL)
	{
		std::cout << "Error: realpath failed" << std::endl;
		close(_client.fd);
		return;
	}
	FILE *file = fopen(_actual_path, "r");
	if (file == NULL)
	{
		std::cout << "Error: fopen failed" << std::endl;
		close(_client.fd);
		return;
	}
	// while (bytes_read = fread(_buffer, 1, sizeof(_buffer), file) > 0)
	// {
	// 	std::cout << "Sending: " << bytes_Read << " bytes" << std::endl;
	// 	write(_client.fd, _buffer, bytes_read);
	// }
	std::fill_n(_buffer, sizeof(_buffer), '\0');
	snprintf(_buffer, sizeof(_buffer), "HTTP/1.1 200 OK\r\n\r\nHello");
	write(_client.fd, _buffer, strlen(_buffer));
	close(_client.fd);
}

void HTTPServer::acceptConnection()
{
	char client_address[BUFFER_SIZE + 1];
	std::cout << "Waiting for connection on port " << SERVER_PORT << std::endl;
	_client.addr_len = sizeof(_client.addr);
	check(_client.fd = accept(_server.fd, (t_sockaddr *)&_client.addr,
							  (socklen_t *)&_client.addr_len),
		  "Error: accept failed");
	inet_ntop(AF_INET, &_client.addr.sin_addr, client_address, sizeof(_buffer));
	std::cout << "Connection received from " << client_address << std::endl;
}
#define TIMEOUT 1000
int HTTPServer::run()
{
	Logger &logger = Logger::getInstance();

	try
	{
		setupServerSocket();
		while (true)
		{
			int poll_count = poll(_fds.data(), _fds.size(), TIMEOUT);
			if (poll_count == -1)
			{
				throw std::runtime_error("poll() failed");
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
				if (fd.revents != POLLIN)
				{
					throw std::runtime_error("revents = " +
											 std::to_string(fd.revents));
				}
				if (fd.fd == _server.fd)
				{
					_thread_pool.QueueTask([&]() { acceptConnection(); });
					_fds.push_back(pollfd{_client.fd, POLLIN, 0});
				}
				else
				{
					_thread_pool.QueueTask([&]() { handleConnection(); });
					close(fd.fd);
					fd.fd = -1;
				}
			}
		}
	}
	catch (const std::exception &e)
	{
		logger.log(FATAL, e.what());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
