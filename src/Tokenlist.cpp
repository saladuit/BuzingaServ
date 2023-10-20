
#include <Logger.hpp>
#include <Token.hpp>
#include <Tokenlist.hpp>

#include <sstream>
#include <string>
#include <vector>

Tokenlist::Tokenlist() : _tokens()
{
}

Tokenlist::~Tokenlist()
{
}

bool SkipComments(std::stringstream &stream, std::string &str)
{
	size_t pos = str.find('#');
	if (pos != std::string::npos)
	{
		std::string filler;
		str.erase(pos);
		getline(stream, filler);
	}
	return (str.empty());
}

// TODO:const std::vector<Token> &getTokens()

TokenType identifyToken(std::string str)
{
	if (str == "}")
		return (TokenType::CLOSE_BRACKET);
	else if (str == "{")
		return (TokenType::OPEN_BRACKET);
	else if (str == ";")
		return (TokenType::SEMICOLON);
	else
		return (TokenType::WORD);
}

std::string breakString(const size_t pos, std::string &input)
{
	std::string token;
	if (pos != 0)
	{
		token = input.substr(0, pos);
		input = input.substr(pos);
	}
	else
	{
		token = input.substr(0, 1);
		input = input.substr(1);
	}
	return (token);
}

#define CHAR_SET ";"

void Tokenlist::splitString(std::string input)
{
	Logger &logger = Logger::getInstance();
	size_t pos = input.find_first_of(CHAR_SET);

	while (pos != std::string::npos)
	{
		std::string item = breakString(pos, input);
		logger.log(DEBUG, "item: " + item);
		_tokens.emplace_back(Token(identifyToken(item), item));
		pos = input.find_first_of(CHAR_SET);
	}
}

void Tokenlist::tokenizeSstream(std::stringstream sstream)
{
	Logger &logger = Logger::getInstance();
	logger.log(INFO, "file_path succesfully opened");
	std::string tmp;

	while (sstream >> tmp)
	{
		if (SkipComments(sstream, tmp))
			continue;
		logger.log(DEBUG, "Token: " + tmp);
		if (tmp.find_first_of(CHAR_SET) != std::string::npos)
			splitString(tmp);
		else
			_tokens.emplace_back(Token(identifyToken(tmp), tmp));
	}

	logger.log(DEBUG, "PRINTING LIST OF TOKENS");

	for (auto &token : _tokens)
		logger.log(DEBUG, "Tokens: " + token.getString());
}
