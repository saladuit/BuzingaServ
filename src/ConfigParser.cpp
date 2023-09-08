#include "ConfigParser.hpp"
#include "Logger.hpp"
#include <fstream>
#include <sstream>

ConfigParser::ConfigParser(const std::string &file_path)
	: _file_path(file_path), _global_settings(), _server_blocks()
{
}

std::map<GlobalSetting, std::string> ConfigParser::getGlobalSettings() const
{
	return (_global_settings);
}

std::vector<ServerBlock> ConfigParser::getServerBlocks() const
{
	return (_server_blocks);
}

std::string stripInlineComments(const std::string &line)
{
	auto commentPosition = line.find('#');
	if (commentPosition != std::string::npos)
		return (line.substr(0, commentPosition));
	return (line);
}

void ConfigParser::parseGlobalBlock(std::istream &stream)
{
	Logger &logger = Logger::getInstance();
	logger.log(LogLevel::DEBUG, "Parsing global block");
	std::string line;
	while (std::getline(stream, line))
	{
		line = stripInlineComments(line);
		if (isCommentOrEmpty(line))
			continue;
		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;
		if (first_word == "server")
		{
			// ServerBlock server_block = parseServerBlock(stream);
		}
	}
}

void ConfigParser::parseServerBlock(std::istream &stream)
{
	Logger &logger = Logger::getInstance();
	logger.log(LogLevel::DEBUG, "Parsing server block");
	ServerBlock server_block;
	std::string line;
	while (std::getline(stream, line))
	{
		line = stripInlineComments(line);
		if (isCommentOrEmpty(line))
			continue;
		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;
		if (first_word == "location")
		{
			// LocationBlock location_block = parseLocationBlock(stream);
		}
	}
	_server_blocks.push_back(server_block);
}

bool ConfigParser::isCommentOrEmpty(const std::string &line)
{
	return (line.empty() || line[0] == '#');
}

void ConfigParser::readConfigFile(std::istream &config_file)
{
	Logger &logger = Logger::getInstance();
	std::string line;

	while (std::getline(config_file, line))
	{
		if (isCommentOrEmpty(line))
			continue;

		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;

		// logger.log(LogLevel::DEBUG, "Parsing block type: " + first_word);
		if (first_word == "server")
			parseServerBlock(config_file);
		else if (first_word == "global")
			parseGlobalBlock(config_file);
		else
			logger.log(LogLevel::WARNING, "Unkown block type: " + first_word);
	}
}
std::ifstream ConfigParser::openConfigFile(const std::string &file_path)
{
	Logger &logger = Logger::getInstance();
	std::ifstream config_file(file_path);

	if (!config_file.is_open())
	{
		logger.log(LogLevel::FATAL, "Could not open config file: " + file_path);
		throw std::runtime_error("Config file could not be opened");
	}

	logger.log(LogLevel::INFO, "Opened config file: " + file_path);
	return (config_file);
}

void ConfigParser::readConfig()
{
	std::ifstream config_file = openConfigFile(_file_path);
	readConfigFile(config_file);
	// validateConfig();
	config_file.close();
}
