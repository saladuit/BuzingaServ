#include "Logger.hpp"
#include <chrono>
#include <iomanip>

Logger::Logger()
	: _current_level(LogLevel::DEBUG),
	  _log_file("build/log/webserver.log", std::ios::trunc | std::ios::out)
{
	if (!_log_file.is_open())
		log(ERROR, "Failed to open log file");
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

void Logger::setLogLevel(const LogLevel lvl)
{
	_current_level = lvl;
}

const std::string Logger::logLevelToString(const LogLevel lvl)
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
		return "ERROR";
	case LogLevel::FATAL:
		return "FATAL";
	default:
		throw std::invalid_argument("Invalid log level");
	}
}

const std::string Logger::getTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	std::ostringstream oss;

	std::tm tm;
	localtime_r(&time, &tm);
	oss << std::put_time(&tm, "%Y-%m-%d %X");
	return oss.str();
}
