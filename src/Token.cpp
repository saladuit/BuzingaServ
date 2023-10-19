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
}

std::ostream &operator<<(std::ostream &os, Token &token)
{
	switch (token.getType())
	{
	case TokenType::SEMICOLON:
		os << ";";
	case TokenType::OPEN_BRACKET:
		os << "{";
	case TokenType::CLOSE_BRACKET:
		os << "}";
	case TokenType::WORD:
		os << "WORD";
	}
	return (os);
}
