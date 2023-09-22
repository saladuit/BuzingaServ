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
		if (lvl > DEBUG)
		{
			std::cout << "[" << getTimestamp() << "] " << logLevelToString(lvl)
					  << ": " << formatted_msg << Color::reset << std::endl;
		}
		if (lvl < _current_level)
			return;
		std::ofstream log_file("log.txt",
							   std::ios_base::app | std::ios_base::out);
		if (!log_file.is_open())
		{
			std::cout << "Failed to open log file" << std::endl;
			return;
		}
		log_file << "[" << getTimestamp() << "] " << logLevelToString(lvl)
				 << ": " << formatted_msg << Color::reset << std::endl;
		log_file.close();
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
