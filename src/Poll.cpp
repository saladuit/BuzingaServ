#include <Logger.hpp>
#include <Poll.hpp>
#include <stdexcept>

Poll::Poll() : _poll_fds()
{
}

Poll::~Poll()
{
}

void Poll::addFD(int fd, short events)
{
	_poll_fds.emplace_back(pollfd{fd, events, 0});
}

void Poll::setEvents(int fd, short events)
{
	auto it = std::find_if(_poll_fds.begin(), _poll_fds.end(),
						   [fd](const pollfd &poll_fd)
						   { return (poll_fd.fd == fd); });
	if (it != _poll_fds.end())
		it->events = events;
}

void Poll::removeFD(int fd)
{
	_poll_fds.erase(std::remove_if(_poll_fds.begin(), _poll_fds.end(),
								   [fd](const pollfd &poll_fd)
								   { return (poll_fd.fd == fd); }),
					_poll_fds.end());
}

std::vector<pollfd> Poll::getFds(void) const
{
	return (_poll_fds);
}

void Poll::pollFDs(void)
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "Polling " + std::to_string(_poll_fds.size()) +
						 " file descriptors");
	int poll_count = poll(_poll_fds.data(), _poll_fds.size(), NO_TIMEOUT);
	if (poll_count == SYSTEM_ERROR || poll_count == 0)
		throw SystemException("poll"); // TODO change poll_count 0 handler
}

std::string Poll::pollEventsToString(short events) const
{
	std::string events_string;
	if (events & POLLIN)
		events_string += " POLLIN";
	if (events & POLLOUT)
		events_string += " POLLOUT";
	if (events & POLLHUP)
		events_string += " POLLHUP";
	if (events & POLLNVAL)
		events_string += " POLLNVAL";
	if (events & POLLPRI)
		events_string += " POLLPRI";
	if (events & POLLRDBAND)
		events_string += " POLLRDBAND";
	if (events & POLLRDNORM)
		events_string += " POLLRDNORM";
	if (events & POLLWRBAND)
		events_string += " POLLWRBAND";
	if (events & POLLWRNORM)
		events_string += " POLLWRNORM";
	if (events & POLLERR)
		events_string += " POLLERR";
	return (events_string);
}
