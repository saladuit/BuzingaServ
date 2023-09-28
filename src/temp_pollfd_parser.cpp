#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

// bool	end_of_headers(std::string search_string) {
// 	for (int i = 0; search_string[i]; i++) {
// 		if (!search_string[i + 3])
// 			return (false);
// 		else if (search_string[i] == '\r' && search_string[i + 1] == '\n' &&
// 				search_string[i + 2] == '\r' && search_string[i + 3] == '\n')
// 			return (true);
// 	}
// 	return (false);
// }

int	get_content_length(std::string search_string) 
{
	const std::string	search_header = "Content-length: ";
	const std::string	end_of_line_delimiter = "\r\n";
	size_t	pos = search_string.find(search_header);
	

	if (pos != std::string::npos) 
	{
		std::string	content_length_value = search_string.substr(pos + search_header.length());
		size_t	end_of_line_pos = content_length_value.find(end_of_line_delimiter);

		if (end_of_line_pos != std::string::npos) 
		{
			std::string	content_value_str = content_length_value.substr(0, end_of_line_pos);
			int	value = std::stoi(content_value_str);
			return (value);
		}
	}
	return (-1);
}

// int	main(void) {
// 	Logger  &logger = Logger::getInstance();
// 	int	fd = open("tests/request/post.txt", O_RDONLY);
// 	char	buffer[4040];
// 	read(fd, buffer, sizeof(buffer));
// 	std::string	_HTTPRequestString = buffer;
// 	logger.log(INFO, _HTTPRequestString);
	

// 	if (end_of_headers(_HTTPRequestString)) 
// 	{
// 		std::cout << "end of headers is reached\n";
// 		if (_HTTPRequestString.substr(0, 4) == "POST") {
// 			std::cout << "this is a post request\n";
// 			int	body_length = get_content_length(_HTTPRequestString);
// 			if (body_length == -1)
// 				std::cout << "404: BAD REQUEST\n";
// 			else
// 				std::cout << "body length: " << body_length << std::endl;
// 		}
// 	}
// 	else
// 		std::cout << "end of headers isn't reached\n";
// 	return 0;
// }


#include <iostream>
#include <string>

bool	is_print(char c)
{
	return ((c >= 32 && c <= 126) || c >= '\n' || c == '\r');
}

bool	handle_connection(pollfd &poll_fd, std::string &_HTTPRequestString, int &content_length, 
			bool &body, size_t &pos, int &content_length_cpy) 
{
	Logger  	&logger = Logger::getInstance();
    int32_t 	read_count = 0;
	const int	buffer_size = 20000;
    char    	buffer[buffer_size];

	(void)content_length;

	if (poll_fd.revents && POLLIN) 
	{
		if (body && content_length <= 0)
		// fd.events |= POLLOUT;
			return (true);
        read_count = read(poll_fd.fd, buffer, buffer_size);
        if (read_count == -1) 
		{
            logger.log(ERROR, "Error: read failed on: " + std::to_string(poll_fd.fd));
			// return ;			
			return (false);
		}
		for (int i = 0; i < buffer_size; i++)
		{
			if (is_print(buffer[i]))
				_HTTPRequestString += buffer[i];
			if (content_length > 0)
				content_length -= buffer_size;
		}
    }
	pos = _HTTPRequestString.find("\r\n\r\n");
	if (!body && pos != std::string::npos)
	{
		if (_HTTPRequestString.substr(0, 4) == "POST") 
		{
			int	body_length = get_content_length(_HTTPRequestString);
			if (body_length == -1)
			// set error status to 404 BAD REQUEST?
			// and output applicable log message
				logger.log(ERROR, "404: BAD REQUEST");
			else
			{
				content_length = body_length - 1;
				content_length_cpy = content_length + 4;
				body = true;
			}
			/ return l
			return (false);
		}
		// fd.events |= POLLOUT;
		return (true);
	}
	return (false);
}

int main(void)
{
    int         fd;
    pollfd      poll;
    bool		exit = false;
	Logger		&logger = Logger::getInstance();
	std::string	_HTTPRequestString;
	int			content_length = 0;
	int			content_length_cpy = 0;
	bool		body = false;
	size_t		pos = 0;
	
    fd = open("tests/request/delete.txt", O_RDONLY);
    if (fd == -1)
        return (1);
    poll.fd = fd;
    poll.revents = 1;
    while (1)
    {
		exit = handle_connection(poll, _HTTPRequestString, content_length, body, pos, content_length_cpy);
		if (exit == true)
			break ;
    }
	std::string this_is_it = _HTTPRequestString.substr(0, pos + content_length_cpy);
	logger.log(INFO, "Whole message:\n\n" + this_is_it);
    return (0);
}
