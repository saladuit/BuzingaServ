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
	void setLogLevel(const LogLevel lvl);

	template <typename... Args>
	void log(const LogLevel lvl, const std::string &message, Args... args)
	{
		const std::string formatted_msg = format(message, args...);
		const std::string ss = "[" + getTimestamp() + "] " +
							   logLevelToString(lvl) + ": " + formatted_msg;
		if (lvl < _current_level)
			return;
		if (lvl == LogLevel::DEBUG)
			std::cout << Color::cyan << ss << Color::reset << std::endl;
		else if (lvl == LogLevel::INFO)
			std::cout << ss << std::endl;
		else if (lvl == LogLevel::WARNING)
			std::cerr << Color::yellow << ss << Color::reset << std::endl;
		else if (lvl == LogLevel::ERROR)
			std::cerr << Color::red << ss << Color::reset << std::endl;
		else if (lvl == LogLevel::FATAL)
			std::cerr << Color::magenta << ss << Color::reset << std::endl;
		if (_log_file.is_open())
			_log_file << ss << std::endl;
	}

  private:
	LogLevel _current_level;
	const std::string logLevelToString(const LogLevel lvl);
	const std::string getTimestamp();
	std::ofstream _log_file;

	template <typename T>
	const std::string any_to_string(const T &value)
	{
		std::ostringstream oss;
		oss << value;
		return (oss.str());
	}
	template <typename... Args>
	const std::string format(const std::string &fmt, Args... args)
	{
		std::ostringstream os;
		const std::array<std::string, sizeof...(Args)> arr{
			any_to_string(args)...};
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
