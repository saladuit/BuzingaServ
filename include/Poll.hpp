#ifndef POLL_HPP
#define POLL_HPP

#include <SystemException.hpp>

#include <sys/poll.h>

#include <algorithm>
#include <ostream>
#include <vector>

#define NO_TIMEOUT -1

class Poll
{
  public:
	Poll();
	Poll(const Poll &other) = delete;
	Poll &operator=(const Poll &rhs) = delete;
	~Poll();

	void addFD(const int fd, const short events);
	void removeFD(const int fd);
	void setEvents(pollfd &pollfd, const short events);
	void pollFDs(void);
	const std::string pollEventsToString(const short events) const;
	const std::vector<pollfd> &getFds(void) const;

  private:
	std::vector<pollfd> _poll_fds;
	void logPollfd(const pollfd &fd) const;
};

#endif
