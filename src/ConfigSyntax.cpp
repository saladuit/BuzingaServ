
#include <ConfigParser.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <stdexcept>
#include <vector>

// Finds the end of the block you are currently at the start of.
std::vector<Token>::iterator findBlockEnd(std::vector<Token> tokenlist,
										  std::vector<Token>::iterator it)
{
	int stack = 1;
	Token &start_block = *it;

	while (stack != 0 && it != tokenlist.end())
	{
		if (it->getType() == TokenType::OPEN_BRACKET)
			stack++;
		else if (it->getType() == TokenType::CLOSE_BRACKET)
			stack--;
		it++;
	}
	if (stack != 0)
		throw std::runtime_error("Syntax Error: Unclosed Block: " +
								 start_block.getString());
	return (it);
}

void skipTokenLine(std::vector<Token>::iterator &it)
{
	while (it->getType() != TokenType::SEMICOLON)
		it++;
}

void syntaxCheckServerBlock(std::vector<Token> tokenlist,
							std::vector<Token>::iterator &it)
{
	Token &block_identifier = *it;
	it++;

	if (it->getType() != TokenType::OPEN_BRACKET)
		throw std::runtime_error("Syntax Error: identified block unopened " +
								 block_identifier.getString());

	const std::vector<Token>::iterator end_block = findBlockEnd(tokenlist, it);
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
