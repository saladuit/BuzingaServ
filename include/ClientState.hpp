#ifndef CLIENTSTATE_HPP
#define CLIENTSTATE_HPP

enum class ClientState
{
	Receiving,
	Loading,
	Sending,
	Done,
	Error,
	Unkown,
};

#endif
