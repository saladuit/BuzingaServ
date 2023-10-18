#include <Client.hpp>

#include <sys/poll.h>

Client::Client(const int &server_fd) : _socket(server_fd)
{
}

Client::~Client()
{
}

int Client::getFD(void) const
{
	return (_socket.getFD());
}

ClientState Client::handleConnection(short events)
{
	Logger &logger = Logger::getInstance();
	(void)events;
	logger.log(INFO, "Handling client connection on fd: " +
						 std::to_string(_socket.getFD()));
	if (events & POLLIN)
	{
		_request.receive(_socket.getFD());
		return (ClientState::WRITE);
	}
	if (events & POLLOUT)
	{
		_response.send(_socket.getFD());
		return (ClientState::DONE);
	}
	/* int32_t read_count = 0; */
	/* const int buffer_size = 1; */
	/* char buffer[buffer_size]; */
	/* HTTPRequest &client(_client_request[poll_fd.fd]); */
	/* FileManager file_manager; */
	/* int status_code = 0; */
	/* logger.log(DEBUG, "remaining content length: %", */
	/* 		   std::to_string(client._content_length)); */
	/* if (poll_fd.revents & POLLIN) */
	/* { */
	/* 	if (client._post_method && client._content_length <= 0) */
	/* 		poll_fd.events |= POLLOUT; */
	/* 	read_count = read(poll_fd.fd, buffer, buffer_size); */
	/* 	if (read_count == -1) */
	/* 	{ */
	/* 		logger.log(ERROR, "Error: read failed on: " +
	 * std::to_string(poll_fd.fd)); */
	/* 		return; */
	/* 	} */
	/* 	logger.log(DEBUG, "bytes read: %", buffer); */
	/* 	logger.log( */
	/* 		INFO, std::to_string(read_count) + 				  " bytes are read
	 * from fd: " + std::to_string(poll_fd.fd)); */
	/* 	for (int i = 0; i < read_count; i++) */
	/* 	{ */
	/* 		if (std::isprint(buffer[i]) || */
	/* 			buffer[i] == '\n' | buffer[i] == '\r') */
	/* 			client._http_request_str += buffer[i]; */
	/* 		if (client._content_length > 0) */
	/* 			client._content_length -= read_count; */
	/* logger.log(DEBUG, "http request str: " + client._http_request_str); */
	/* 	} */
	/* } */
	/* client._pos = client._http_request_str.find("\r\n\r\n"); */
	/* logger.log(DEBUG, "1"); */
	/* if (!client._post_method && client._pos != std::string::npos) */
	/* { */
	/* 	logger.log(DEBUG, "2"); */
	/* 	if (client._http_request_str.substr(0, 4) == "POST") */
	/* 	{ */
	/* 		int body_length = get_content_length(client._http_request_str); */
	/* 		if (body_length == -1) */
	/* 			// set error status to 404 BAD REQUEST? */
	/* 			// and output applicable log message */
	/* 			logger.log(ERROR, "404: BAD REQUEST"); */
	/* 		else */
	/* 		{ */
	/* 			// body_length + 2 works */
	/* 			client._content_length = body_length - 2; */
	/* 			client._content_length_cpy = client._content_length; */
	/* 			client._post_method = true; */
	/* 		} */
	/* 		return; */
	/* 	} */
	/* 	poll_fd.events |= POLLOUT; */
	/* } */
	/* if (poll_fd.revents & POLLOUT) */
}
