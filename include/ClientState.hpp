#ifndef CLIENTSTATE_HPP
#define CLIENTSTATE_HPP

enum class ClientState
{
	Receiving,
	CGI_Start,
	CGI_Write,
	CGI_Read,
	CGI_Load,
	Loading,
	Sending,
	Done,
	Error,
	Unkown,
};

#endif
