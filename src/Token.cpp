#include "Token.hpp"

#include <sstream>

Token::Token(TokenType type, const std::string &string)
	: _type(type), _string(string)
{
}

Token::~Token()
{
}

Token::Token(const Token &src) : _type(src._type), _string(src._string)
{
}

const std::string &Token::getString() const
{
	return (_string);
}

TokenType Token::getType() const
{
	return (_type);
}

std::string Token::typeToString() const
{
	switch (_type)
	{
	case TokenType::SEMICOLON:
		return ("SEMICOLON");
	case TokenType::OPEN_BRACKET:
		return ("OPEN_BRACKET");
	case TokenType::CLOSE_BRACKET:
		return ("CLOSE_BRACKET");
	case TokenType::WORD:
		return ("WORD");
	}
	return ("INCORRECT CONVERTION");
}

const std::string operator+(std::string &lhs, const Token &rhs)
{
	switch (rhs.getType())
	{
	case TokenType::SEMICOLON:
		return (lhs += ";");
	case TokenType::OPEN_BRACKET:
		return (lhs += "{");
	case TokenType::CLOSE_BRACKET:
		return (lhs += "}");
	case TokenType::WORD:
		return (lhs += rhs.getString());
	}
	return ("INCORRECT CONVERTION");
}
