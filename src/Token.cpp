#include "Token.hpp"

#include <sstream>

Token::Token(const TokenType type, const std::string &string)
	: _type(type), _string(string)
{
}

const std::string Token::getString() const
{
	return (_string);
}

TokenType Token::getType() const
{
	return (_type);
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
		os << "{";
	case TokenType::WORD:
		os << "{";
	}
	return (os);
}

std::ostringstream &operator<<(std::ostringstream &os, const Token &token)
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
