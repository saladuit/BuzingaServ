#include "Logger.hpp"
#include <sstream>

Logger::Logger() : _current_level(LogLevel::INFO)
{
}

Logger::~Logger()
{
}

void Logger::setLogLevel(LogLevel lvl)
{
	_current_level = lvl;
}

std::string Logger::logLevelToString(LogLevel lvl)
{
	switch (lvl)
	{
	case LogLevel::DEBUG:
		return Color::cyan + "DEBUG";
	case LogLevel::INFO:
		return Color::white + "INFO";
	case LogLevel::WARNING:
		return Color::yellow + "WARNING";
	case LogLevel::ERROR:
		return Color::red + "ERROR";
	case LogLevel::FATAL:
		return Color::magenta + "FATAL";
	default:
		throw std::invalid_argument("Invalid log level");
	}
}

template <typename... Args>
void Logger::log(LogLevel lvl, const char *message, Args... args)
{
	std::string formatted_msg = format(message, args...);
	if (lvl < _current_level)
		return;

	std::cout << "[" << getTimestamp() << "]" << logLevelToString(lvl) << ": "
			  << formatted_msg << Color::reset << std::endl;
}

template <typename... Args>
std::string Logger::format(std::string &fmt, Args... args)
{
	std::ostringstream os;
	size_t i;

	i = 0;
	for (char c : fmt)
	{
		if (c == '%')
		{
			std::array<std::string, sizeof...(Args)> arr(
				std::to_string(args)...);
			os << arr[i++];
		}
		else
		{
			os << c;
		}
	}
	return (os.str());
}

std::string Logger::getTimestamp()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return (buf);
}
