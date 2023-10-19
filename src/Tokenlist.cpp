
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

// TODO: const std::vector<Token> &getTokens();

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

std::vector<Token> splitStringToList(std::string input)
{
	Logger &logger = Logger::getInstance();
	std::vector<Token> list;
	size_t pos = input.find_first_of("{};");

	while (pos != std::string::npos)
	{

		std::string item(input.substr(0, pos));
		logger.log(DEBUG, "input: " + input);
		logger.log(DEBUG, "item: " + item);
		logger.log(DEBUG, "pos: " + std::to_string(pos));
		input.substr(pos);
		list.push_back(Token(identifyToken(item), item));
		pos = input.find_first_of("{};");
	}
	return (list);
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
		Token token(identifyToken(tmp), tmp);
		logger.log(DEBUG, "\n\tToken:\t\t|" + token.getString() +
							  "|\n\tTokenType:\t" + token.typeToString());
		_tokens.push_back(token);
		/* std::vector<Token> list; */
		/* list = splitStringToList(tmp); */
		/* for (auto &item : list) */
		/* 	_tokens.push_back(item); */
	}
}
