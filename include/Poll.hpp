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

	void addFD(int fd, short events);
	void removeFD(int fd);
	void setEvents(int fd, short events);
	void pollFDs(void);
	std::string pollEventsToString(short events) const;
	std::vector<pollfd> getFds(void) const;

  private:
	std::vector<pollfd> _poll_fds;
};

#endif
