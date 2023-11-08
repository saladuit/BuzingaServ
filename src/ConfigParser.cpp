
#include <ConfigParser.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>
#include <Token.hpp>

#include <filesystem>
#include <ios>
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
	// syntaxCheck(tokenlist);

	for (std::vector<Token>::iterator it = tokenlist.begin();
		 it != tokenlist.end(); it++)
		_server_settings.emplace_back(ServerSettings(it));

	for (auto &it : _server_settings)
		it.printServerSettings();

	logger.log(INFO, "Parsed configfile: " + _config_file_path);
}
