#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <ostream>

enum class TokenType
{
	SEMICOLON,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	WORD,
};

class Token
{
  public:
	Token(const TokenType type, const std::string &string);

	TokenType getType() const;
	const std::string getString() const;

  private:
	TokenType _type;
	std::string _string;
};

std::ostream &operator<<(std::ostream &os, Token &token);
std::ostringstream &operator<<(std::ostringstream &os, const Token &token);
#endif
