#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <array>
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <string>

#include "Color.hpp"

enum LogLevel
{
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL,
};

class Logger
{
  public:
	Logger();
	Logger(Logger const &) = delete;
	void operator=(Logger const &) = delete;
	~Logger();

	static Logger &getInstance();
	void setLogLevel(LogLevel lvl);
	template <typename... Args>
	void log(LogLevel lvl, const std::string &message, Args... args)
	{
		if (lvl < _current_level)
			return;

		std::string formatted_msg = format(message, args...);
		std::cout << "[" << getTimestamp() << "]" << logLevelToString(lvl)
				  << ": " << formatted_msg << Color::reset << std::endl;
	}

  private:
	LogLevel _current_level;
	std::string logLevelToString(LogLevel lvl);
	std::string getTimestamp();

	template <typename T>
	std::string any_to_string(const T &value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}
	template <typename... Args>
	std::string format(const std::string &fmt, Args... args)
	{
		std::ostringstream os;
		std::array<std::string, sizeof...(Args)> arr{any_to_string(args)...};
		size_t i;

		i = 0;
		for (char c : fmt)
		{
			if (c == '%')
			{
				os << arr[i++];
			}
			else
			{
				os << c;
			}
		}
		return (os.str());
	}
};

#endif
