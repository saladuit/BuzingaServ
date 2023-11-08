
#include <ConfigParser.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <stdexcept>
#include <vector>

std::vector<Token>::iterator findServerBlockEnd(std::vector<Token>::iterator it)
{
	return (it);
}

void skipTokenLine(std::vector<Token>::iterator &it)
{
	while (it->getType() != TokenType::SEMICOLON)
		it++;
}

void identifyLocationBlocks(std::vector<Token> tokenlist,
							std::vector<Token>::iterator it,
							const std::vector<Token>::iterator endBlock)
{

	while (it != endBlock)
	{
		if (it->getString() != "location")
			skipTokenLine(it);
		else
		{
			it++;
			it->setType(TokenType::PATH);
		}
		it++;
	}
}

void syntaxCheckServerBlock(std::vector<Token> tokenlist,
							std::vector<Token>::iterator &it)
{
	it++;

	const std::vector<Token>::iterator endBlock = findServerBlockEnd(it);

	identifyLocationBlocks(tokenlist, it, endBlock);
}

void ConfigParser::syntaxCheck(std::vector<Token> tokenlist)
{
	std::vector<Token>::iterator it = tokenlist.begin();

	while (it != tokenlist.end())
	{
		if (it->getString() == "server")
			syntaxCheckServerBlock(tokenlist, it);
		else
			throw std::runtime_error(
				"Syntax Error: unknown block identifier: " + it->getString());
		it++;
	}
}
