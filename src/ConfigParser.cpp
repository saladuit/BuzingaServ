
#include "ServerSetting.hpp"
#include <ConfigParser.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <filesystem>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

ConfigParser::ConfigParser(const std::string &file_path)
	: _config_file_path(file_path), _tokenlist(), _server_settings()
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "ConfigParser Constructor called " + _config_file_path);
}

ConfigParser::~ConfigParser()
{
}

const std::vector<ServerSetting> &ConfigParser::getServerSettings()
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

	logger.log(INFO, "Parsing Config File");
	_tokenlist.tokenizeSstream(OpenFile());
	logger.log(INFO, "tokenlist succesfully made");
}
