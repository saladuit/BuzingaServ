#ifndef SERVER_HPP
#define SERVER_HPP

// following IANA regulations port 80 is dedicated for web servers.
#define PORT_NUM 80

typedef enum e_method_index
{
	GET,
	POST,
	DELETE,
	ERROR,
}	t_method_index;

#endif
