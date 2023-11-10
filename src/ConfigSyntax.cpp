
#include <ConfigParser.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <stdexcept>
#include <vector>

void printLine(std::vector<Token>::iterator it)
{
	Logger &logger = Logger::getInstance();

	std::string line;
	while (it->getType() == TokenType::WORD)
	{
		line += " " + it->getString();
		it++;
	}
	logger.log(DEBUG, line);
}

// Finds the end of the block. expects to be called at block_identifier token.
std::vector<Token>::iterator findBlockEnd(std::vector<Token> tokenlist,
										  std::vector<Token>::iterator it)
{
	int stack;
	Token &start_block = *it;

	it++;
	if (start_block.getString() == "location")
		it++;
	if (it->getType() != TokenType::OPEN_BRACKET)
		throw std::runtime_error("Syntax Error: identified block unopened " +
								 start_block.getString());
	stack = 1;
	it++;

	while (it != tokenlist.end() && stack != 0)
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

void syntaxCheckLine(const Token &block_identifier,
					 std::vector<Token>::iterator &it)
{
	size_t words = 0;

	while (it->getType() == TokenType::WORD)
	{
		words++;
		it++;
	}
	if (words <= 1)
		throw std::runtime_error(
			"Syntax Error: Line doesn't contain KEY:VALUE pair in block: " +
			block_identifier.getString());
	else if (it->getType() != TokenType::SEMICOLON)
		throw std::runtime_error(
			"Syntax Error: Line doesn't end with ';' in block: " +
			block_identifier.getString());
}

void syntaxCheckRequestTarget(std::string path)
{
	(void)path;
}

void syntaxCheckLocationBlock(std::vector<Token> tokenlist,
							  std::vector<Token>::iterator &it)
{
	const Token &block_identifier = *it;
	const std::vector<Token>::iterator end_block = findBlockEnd(tokenlist, it);

	Logger &logger = Logger::getInstance();
	logger.log(INFO, end_block->getString());
	it++;
	syntaxCheckRequestTarget(it->getString());
	it += 2;

	while (it != end_block)
	{
		printLine(it);
		syntaxCheckLine(block_identifier, it);
		it++;
	}
}

void syntaxCheckServerBlock(std::vector<Token> tokenlist,
							std::vector<Token>::iterator &it)
{
	const Token &block_identifier = *it;
	const std::vector<Token>::iterator end_block = findBlockEnd(tokenlist, it);
	it += 2;

	while (it != end_block)
	{
		printLine(it);
		if (it->getString() == "location")
			syntaxCheckLocationBlock(tokenlist, it);
		else
			syntaxCheckLine(block_identifier, it);
		it++;
	}
}

void ConfigParser::syntaxCheck(std::vector<Token> tokenlist)
{
	std::vector<Token>::iterator it = tokenlist.begin();

	while (it != tokenlist.end())
	{
		printLine(it);
		if (it->getString() == "server")
			syntaxCheckServerBlock(tokenlist, it);
		else
			throw std::runtime_error(
				"Syntax Error: unknown block identifier: " + it->getString());
		it++;
	}
}
