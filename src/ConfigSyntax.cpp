
#include <ConfigParser.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<Token>::iterator findBlockEnd(std::vector<Token> &tokens,
										  size_t pos)
{
	std::vector<Token>::iterator it = tokens.begin() + pos;
	std::vector<Token>::iterator blockidentifier = it;

	it++;
	if (blockidentifier->getString() == "location")
		it++; // to skip the directory specified in locationblock

	if (it->getType() != TokenType::OPEN_BRACKET)
		throw std::runtime_error("Syntax Error: '" +
								 blockidentifier->getString() +
								 "' Block isn't opened");
	it++;

	size_t stack = 1;
	while (it != tokens.end() && stack != 0)
	{
		it++;
		if (it->getType() == TokenType::OPEN_BRACKET)
			stack++;
		else if (it->getType() == TokenType::CLOSE_BRACKET)
			stack--;
	}

	if (stack != 0)
		throw std::runtime_error(
			"Syntax Error: " + blockidentifier->getString() +
			" Block isn't closed");

	return (it);
}

void syntaxRequestTarget(std::vector<Token>::iterator &it)
{
	const std::string reqtarget = it->getString();

	if (reqtarget.find_first_of("/") != 0)
		throw std::runtime_error("Syntax Error: locationBlock RequestTarget "
								 "doesn't start with '/' : " +
								 reqtarget);
	if (reqtarget.length() == 1)
		return;

	if (reqtarget.find_last_of("/") != reqtarget.length() - 1)
		throw std::runtime_error("Syntax Error: locationBlock RequestTarget "
								 "doesn't end with '/' : " +
								 reqtarget);
}

void syntaxLine(std::vector<Token>::iterator &it)
{
	if (it->getType() != TokenType::WORD)
		throw std::runtime_error(
			"Syntax Error: Line doesn't start with TokenType WORD" +
			it->getString());
	Token &lineidentifier = *it;
	size_t words = 0;

	while (it->getType() == TokenType::WORD)
	{
		words++;
		it++;
	}

	if (words <= 1)
		throw std::runtime_error(
			"Syntax Error: Line doesn't contain KEY:VALUE pair in block: " +
			lineidentifier.getString());
	else if (it->getType() != TokenType::SEMICOLON)
		throw std::runtime_error(
			"Syntax Error: Line doesn't end with ';' in block. near token: " +
			lineidentifier.getString());
}

void syntaxLocationBlock(std::vector<Token> &tokens,
						 std::vector<Token>::iterator &it)
{
	std::vector<Token>::iterator closing_token =
		findBlockEnd(tokens, std::distance(tokens.begin(), it));

	if (it->getString() != "location")
		throw std::runtime_error(
			"Syntax Error: locationBlock starts with unfamiliar identifier: " +
			it->getString());
	it++;
	syntaxRequestTarget(it);
	it += 2;

	while (it != closing_token)
	{

		syntaxLine(it);
		it++;
	}
}

void syntaxServerBlock(std::vector<Token> &tokens,
					   std::vector<Token>::iterator &it)
{
	std::vector<Token>::iterator closing_token;
	if (it == tokens.begin())
		closing_token = findBlockEnd(tokens, 0);
	else
		closing_token = findBlockEnd(tokens, std::distance(tokens.begin(), it));

	if (it->getString() != "server")
		throw std::runtime_error(
			"Syntax Error: ServerBlock starts with unfamiliar identifier: " +
			it->getString());
	it += 2;

	while (it != closing_token)
	{
		if (it->getString() == "location")
			syntaxLocationBlock(tokens, it);
		else
			syntaxLine(it);
		it++;
	}
}

void ConfigParser::syntax(std::vector<Token> tokenlist)
{
	std::vector<Token>::iterator it = tokenlist.begin();

	while (it != tokenlist.end())
	{
		if (it->getString() == "server")
			syntaxServerBlock(tokenlist, it);
		else
			throw std::runtime_error(
				"Syntax Error: unknown block identifier: " + it->getString());
		it++;
	}
}
