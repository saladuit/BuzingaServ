
#include <ConfigParser.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>
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

	_tokenlist.tokenizeSstream(OpenFile());

	for (std::vector<Token>::iterator it = _tokenlist.getTokens().begin();
		 it != _tokenlist.getTokens().end(); it++)
		_server_settings.emplace_back(ServerSettings(it));

	for (auto &it : _server_settings)
		it.printServerSettings();

	logger.log(INFO, "Parsered configfile: " + _config_file_path);
}
