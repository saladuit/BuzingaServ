
#include <ConfigParser.hpp>
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>
#include <Token.hpp>

#include <filesystem>
#include <iterator>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <vector>

ConfigParser::ConfigParser(const std::string &file_path)
	: _config_file_path(file_path), _server_settings()
{
}

ConfigParser::~ConfigParser()
{
}

void ConfigParser::sortServerSettings()
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "sortServerSettings");

	std::map<std::string, std::vector<ServerSettings>> map_servers;

	for (ServerSettings block : _server_settings)
	{
		std::map<std::string, std::vector<ServerSettings>>::iterator it =
			map_servers.find(block.getListen());
		if (it == map_servers.end())
		{
			std::vector<ServerSettings> vec;
			vec.push_back(block);
			map_servers.emplace(block.getListen(), vec);
		}
		else
			it->second.emplace_back(block);
	}

	/*
	for (auto vec : map_servers)
	{
		logger.log(DEBUG, "HOST:PORT\t" + vec.second.at(0).getListen());
		for (auto block : vec.second)
			block.printServerSettings();
	}
	exit(10);
	*/
}

const std::vector<ServerSettings> &ConfigParser::getServerSettings()
{
	return (_server_settings);
}

std::stringstream ConfigParser::OpenFile()
{
	if (std::filesystem::is_character_file(_config_file_path))
		throw std::runtime_error(
			"OpenFile: given file_path is not a character_file: " +
			_config_file_path);

	std::ifstream file_stream(_config_file_path);
	if (!file_stream.is_open())
		throw std::runtime_error("OpenFile: given file_path can't be opened: " +
								 _config_file_path);
	std::stringstream str_stream;
	str_stream << file_stream.rdbuf();

	return (str_stream);
}

void ConfigParser::ParseConfig()
{
	Logger &logger = Logger::getInstance();
	std::vector<Token> tokenlist;

	tokenizeStream(OpenFile(), tokenlist);

	syntax(tokenlist);
	logger.log(INFO, "Passed Syntax " + _config_file_path);

	for (std::vector<Token>::iterator it = tokenlist.begin();
		 it != tokenlist.end(); it++)
		_server_settings.emplace_back(ServerSettings(it));

	logger.log(INFO, "Parsed configfile: " + _config_file_path);
}
