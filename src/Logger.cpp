#include "Logger.hpp"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::log(LogLevel lvl, const char *message, ...)
{
	va_list args;
	va_start(args, message);
	std::string msg = message;
	std::string lvlStr = logLevelToString(lvl);
	std::string formattedMsg = format(msg, args);
	va_end(args);
	std::cout << "[" << getTimestamp() << "] " << lvlStr << ": " << formattedMsg
			  << Color::reset << std::endl;
}

std::string logLevelToString(LogLevel lvl)
{
	switch (lvl)
	{
	case LOG_DEBUG:
		return Color::cyan + "DEBUG";
	case LOG_INFO:
		return Color::white + "INFO";
	case LOG_WARNING:
		return Color::yellow + "WARNING";
	case LOG_ERROR:
		return Color::red + "ERROR";
	case LOG_FATAL:
		return Color::magenta + "FATAL";
	default:
		throw std::invalid_argument("Invalid log level");
	}
}

std::string Logger::format(std::string msg, va_list args)
{
	char *buffer;
	std::size_t size;

	buffer = NULL;
	size = vsnprintf(buffer, 0, msg.c_str(), args);
	buffer = new char[size + 1];
	vsnprintf(buffer, size + 1, msg.c_str(), args);
	std::string formatted_msg = buffer;
	delete[] buffer;
	return (formatted_msg);
}

std::string Logger::getTimestamp()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return (buf);
}
