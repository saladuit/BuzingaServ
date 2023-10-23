
#include <Logger.hpp>
#include <Token.hpp>
#include <Tokenlist.hpp>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

Tokenlist::Tokenlist() : _tokens()
{
}

Tokenlist::~Tokenlist()
{
}

std::vector<Token> &Tokenlist::getTokens()
{
	return (_tokens);
}

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

#ifndef TOKENLIST_SPECIAL_CHAR
#define TOKENLIST_SPECIAL_CHAR "{};"
#endif

void stringToLower(std::string &str)
{
	for (size_t i = 0; i != str.size(); i++)
		str.at(i) = std::tolower(str.at(i));
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

void Tokenlist::splitString(std::string input)
{
	size_t pos = input.find_first_of(TOKENLIST_SPECIAL_CHAR);

	while (pos != std::string::npos)
	{
		std::string token;
		token = input.substr(0, (pos != 0 ? pos : 1));
		input = input.substr(pos != 0 ? pos : 1);
		_tokens.emplace_back(Token(identifyToken(token), token));
		pos = input.find_first_of(TOKENLIST_SPECIAL_CHAR);
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
		//		stringToLower(tmp); // Might be usefull to have. BUT we a PATH
		//is case sensitive so we'd have to do this in the syntax checker and it
		// should exclude PATHs
		if (tmp.find_first_of(TOKENLIST_SPECIAL_CHAR) != std::string::npos)
			splitString(tmp);
		else
			_tokens.emplace_back(Token(identifyToken(tmp), tmp));
	}
	for (auto &token : _tokens)
		logger.log(DEBUG, "TOKEN: " + token.getString());
}