#ifndef POLL_HPP
#define POLL_HPP

#include <SystemException.hpp>

#include <sys/poll.h>

#include <algorithm>
#include <vector>

#define NO_TIMEOUT (-1)

class Poll
{
  public:
	Poll();
	Poll(const Poll &other) = delete;
	Poll &operator=(const Poll &rhs) = delete;
	~Poll();

	void addPollFD(int fd, short events);
	bool pollFDs(void);
	void removeFD(int fd);
	void setEvents(int fd, short events);
	void checkREvents(short revents) const;

	std::string pollEventsToString(short events) const;
	std::vector<pollfd> getPollFDs(void) const;

	class PollException : public std::runtime_error
	{
	  public:
		PollException(const std::string &message) : std::runtime_error(message)
		{
		}
	};

  private:
	std::vector<pollfd> _poll_fds;
};

#endif
