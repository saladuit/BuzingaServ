#include "ConfigParser.hpp"
#include "Logger.hpp"
#include "Token.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

ConfigParser::ConfigParser(const std::string &file_path)
	: _configfile_path(file_path), _global_settings(), _server_blocks()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "ConfigParser: Start Tokenzier");
	tokenizeConfigfile(_config_file);

	logger.log(INFO, "ConfigParser: ");
}

const std::string &
ConfigParser::getGlobalSettings(const GlobalSetting setting) const
{
	return (_global_settings.at(setting));
}

const std::vector<ServerBlock> &ConfigParser::getServerBlocks() const
{
	return (_server_blocks);
}

bool skip_comments(std::ifstream &stream, std::string &token)
{

	size_t pos = token.find('#');
	if (pos != std::string::npos)
	{
		std::string filler;
		token.erase(pos);
		getline(stream, filler);
		return (true);
	}
	return (false);
}

void ConfigParser::tokenizeConfigfile(std::ifstream &stream)
{
	Logger &logger = Logger::getInstance();
}
