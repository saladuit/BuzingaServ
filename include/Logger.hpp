#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <array>
#include <cstdarg>
#include <fstream>
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
	~Logger();

	Logger(Logger const &) = delete;
	void operator=(Logger const &) = delete;

	static Logger &getInstance();
	void setLogLevel(LogLevel lvl);

	template <typename... Args>
	void log(LogLevel lvl, const std::string &message, Args... args)
	{
		std::string formatted_msg = format(message, args...);
		if (lvl < _current_level)
			return;
		if (!_log_file.is_open())
		{
			std::cout << "Failed to open log file" << std::endl;
			return;
		}
		std::cout << "[" << getTimestamp() << "] " << logLevelToString(lvl)
				  << ": " << formatted_msg << Color::reset << std::endl;
		_log_file << "[" << getTimestamp() << "] " << logLevelToStringFile(lvl)
				  << ": " << formatted_msg << std::endl;
	}

  private:
	LogLevel _current_level;
	std::string logLevelToString(LogLevel lvl);
	std::string logLevelToStringFile(LogLevel lvl);
	std::string getTimestamp();
	std::ofstream _log_file;

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
