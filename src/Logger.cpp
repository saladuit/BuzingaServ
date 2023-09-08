#include "Logger.hpp"
#include <iomanip>

Logger::Logger() : _current_level(LogLevel::DEBUG)
{
}

Logger::~Logger()
{
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

std::string Logger::getTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);

	std::tm tm;
	localtime_r(&time, &tm);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %X");

	return oss.str();
}
