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
	: _config_file(file_path), _global_settings(), _server_blocks()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "ConfigParser: Start Tokenzier");
	tokenizeConfigfile(_config_file);
	TokenValidator();
	TokenParser();

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

void ConfigParser::ParseServerBlock(size_t &i)
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Parse" + _tokens.at(i).getString() + "Block ");
	while (_tokens.at(i).getType() != TokenType::CLOSE_BRACKET)
	{
	}
}

void ConfigParser::ParseGlobalBlock(size_t &i)
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Parse" + _tokens.at(i).getString() + "Block ");
	i += 2;
	do
	{
		Token &key = _tokens.at(i);
		Token &value = _tokens.at(i + 1);
		logger.log(DEBUG, "Found GlobalSetting: KEY:\t" + key.getString() +
							  "\tVAL:\t" + value.getString());
		if (key.getString() == "threads")
			_global_settings[GlobalSetting::Threads] = value.getString();
		else if (key.getString() == "default_error_pages")
			_global_settings[GlobalSetting::DefaultErrorPages] =
				value.getString();
		else if (key.getString() == "read_size")
			_global_settings[GlobalSetting::ReadSize] = value.getString();
		else if (key.getString() == "write_size")
			_global_settings[GlobalSetting::WriteSize] = value.getString();
		else
			logger.log(WARNING, "Unknown GlobalSetting");
		while (_tokens.at(i).getType() != TokenType::SEMICOLON)
			i++;
		i++;
	} while (_tokens.at(i).getType() != TokenType::CLOSE_BRACKET);
}

void ConfigParser::TokenParser()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "ConfigParser: TokenParser:");

	for (size_t i = 0; i < _tokens.size(); i++)
	{
		Token &token = _tokens.at(i);
		if (token.getString() == "global" && _global_settings.empty())
			ParseGlobalBlock(i);
		else if (token.getString() == "server")
			ParseServerBlock(i);
		else
			logger.log(WARNING, "ConfigParser: Unknown Block ConfigFile");
	}
}

void ConfigParser::TokenValidator(void)
{
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
	std::string token;
	logger.log(DEBUG, "Tokenizing Configfile" + token);
	while (stream >> token)
	{
		if (skip_comments(stream, token))
			continue;
		else if (token == "{")
			_tokens.push_back(Token(TokenType::OPEN_BRACKET, token));
		else if (token == "}")
			_tokens.push_back(Token(TokenType::CLOSE_BRACKET, token));
		else if (token == ";")
			_tokens.push_back(Token(TokenType::SEMICOLON, token));
		else
			_tokens.push_back(Token(TokenType::WORD, token));
	}
}
