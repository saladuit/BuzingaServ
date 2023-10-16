#include <Client.hpp>

Client::Client(const int &fd) : _socket(fd)
{
}

// call read on the fd for a specified byte_size
// add these bytes to the _HTTPRequestString of the HTTPRequest class
// check if everything of the http request has been read
// 		reached /r/n/r/n
//		optionally has read the body (POST request)
// if so, call http parser, file_manager and response
// if not, exit

int Client::getFD(void) const
{
	return (_socket.getFD());
}

void Client::handleConnection(pollfd &poll_fd)
{
	(void)poll_fd;
	/* Logger &logger = Logger::getInstance(); */
	/* int32_t read_count = 0; */
	/* const int buffer_size = 1; */
	/* char buffer[buffer_size]; */
	/* HTTPRequest &client(_client_request[poll_fd.fd]); */
	/* FileManager file_manager; */
	/* int status_code = 0; */
	/**/
	/* logger.log(DEBUG, "remaining content length: %", */
	/* 		   std::to_string(client._content_length)); */
	/* if (poll_fd.revents & POLLIN) */
	/* { */
	/* 	if (client._post_method && client._content_length <= 0) */
	/* 		poll_fd.events |= POLLOUT; */
	/* 	read_count = read(poll_fd.fd, buffer, buffer_size); */
	/* 	if (read_count == -1) */
	/* 	{ */
	/* 		logger.log(ERROR, */
	/* 				   "Error: read failed on: " + std::to_string(poll_fd.fd));
	 */
	/* 		return; */
	/* 	} */
	/* 	logger.log(DEBUG, "bytes read: %", buffer); */
	/* 	logger.log( */
	/* 		INFO, std::to_string(read_count) + */
	/* 				  " bytes are read from fd: " + std::to_string(poll_fd.fd));
	 */
	/* 	for (int i = 0; i < read_count; i++) */
	/* 	{ */
	/* 		if (std::isprint(buffer[i]) || */
	/* 			buffer[i] == '\n' | buffer[i] == '\r') */
	/* 			client._http_request_str += buffer[i]; */
	/* 		if (client._content_length > 0) */
	/* 			client._content_length -= read_count; */
	/* 		logger.log(DEBUG, "http request str: " + client._http_request_str);
	 */
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
	/* { */
	/* 	client.parse(); */
	/* 	logger.log(DEBUG, "path: " + client.getPath()); */
	/* 	logger.log(DEBUG, "version " + client.getVersion()); */
	/* 	logger.log(DEBUG, "Header[\"Host\"]: " + client.getValue("Host")); */
	/* 	logger.log(DEBUG, "Body: " + client.getBody()); */
	/**/
	/* 	// FileManager class */
	/* 	logger.log(INFO, "HTTP file manager"); */
	/* 	file_manager.manage(client.getMethodType(), client.getPath(), */
	/* 						client.getBody()); */
	/* 	status_code = file_manager.getStatusCode(); */
	/* 	logger.log(DEBUG, "_content: " + file_manager.getContent()); */
	/* 	logger.log(DEBUG, "_status_code after calling file manager is: %", */
	/* 			   std::to_string(status_code)); */
	/* 	logger.log(INFO, "HTTP response"); */
	/**/
	/* 	// HTTPResponse class */
	/* 	HTTPResponse response(client.getVersion(), file_manager.getStatusCode(),
	 */
	/* 						  file_manager.getContent()); */
	/* 	response.setHeader("Content-type", client.getValue("Content-type")); */
	/* 	if (client.getMethodType() == HTTPMethod::GET) */
	/* 	{ */
	/* 		response.setHeader("Content-length", */
	/* 						   std::to_string(response.getBody().length())); */
	/* 	} */
	/* 	response.createHTTPResponse(); */
	/**/
	/* 	write(poll_fd.fd, response.getHTTPResponse().c_str(), */
	/* 		  response.getHTTPResponse().size()); */
	/* 	close(poll_fd.fd); */
	/* 	_fds.erase(std::remove_if(_fds.begin(), _fds.end(), */
	/* 							  [&](const pollfd &pfd) */
	/* 							  { return (pfd.fd == poll_fd.fd); }), */
	/* 			   _fds.end()); */
}
