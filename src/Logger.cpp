#include "Logger.hpp"
#include <chrono>
#include <iomanip>

Logger::Logger()
	: _current_level(LogLevel::DEBUG),
	  _log_file("log.txt", std::ios::trunc | std::ios::out)
{
}

Logger::~Logger()
{
	_log_file.close();
}

Logger &Logger::getInstance()
{
	static Logger instance;
	return (instance);
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
		return "INFO";
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
std::string Logger::logLevelToStringFile(LogLevel lvl)
{
	switch (lvl)
	{
	case LogLevel::DEBUG:
		return "DEBUG";
	case LogLevel::INFO:
		return "INFO";
	case LogLevel::WARNING:
		return "WARNING";
	case LogLevel::ERROR:
		return "ERROR ";
	case LogLevel::FATAL:
		return "FATAL";
	default:
		throw std::invalid_argument("Invalid log level");
	}
}
std::string Logger::getTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	std::ostringstream oss;

	std::tm tm;
	localtime_r(&time, &tm);
	oss << std::put_time(&tm, "%Y-%m-%d %X");
	return oss.str();
}
