#ifndef SERVER_HPP
#define SERVER_HPP

// INCLUDES
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <poll.h>

// STRUCTURES
typedef enum e_method_index
{
	GET,
	POST,
	DELETE,
	ERROR,
}	t_method_index;

//typedef struct s_client_information
//{
//
//}	t_client_information;

#endif
