#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdarg>
#include <iostream>
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

	void setLogLevel(LogLevel lvl);
	template <typename... Args>
	void log(LogLevel lvl, const char *message, Args... args);

  private:
	Logger &operator=(const Logger &src);
	Logger(const Logger &src);

	LogLevel _current_level;
	std::string logLevelToString(LogLevel lvl);
	std::string getTimestamp();

	template <typename... Args>
	std::string format(std::string &fmt, Args... args);
};

#endif
