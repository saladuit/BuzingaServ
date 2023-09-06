#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdarg>
#include <iostream>
#include <string>

#include "Color.hpp"

enum LogLevel
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL,
};

class Logger
{

  public:
	Logger();
	~Logger();

	static void log(LogLevel lvl, const char *message, ...);

  private:
	Logger &operator=(const Logger &src);
	Logger(const Logger &src);

	static std::string logLevelToString(LogLevel lvl);
	static std::string format(std::string msg, va_list args);
	static std::string getTimestamp();
};

#endif
