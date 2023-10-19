#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <ostream>
#include <sstream>
#include <vector>

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
	~Token();
	Token() = delete;
	Token(const Token &src);

	TokenType getType() const;
	const std::string &getString() const;
	std::string showTokenType() const;

  private:
	const TokenType _type;
	const std::string &_string;
};

std::ostream &operator<<(std::ostream &os, Token &token);
std::ostringstream &operator<<(std::ostringstream &os, const Token &token);
#endif
