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
	Token(TokenType type, const std::string &string);
	~Token();
	Token(const Token &rhs);
	Token() = delete;
	const Token &operator=(const Token &rhs) = delete;

	TokenType getType() const;
	const std::string &getString() const;
	std::string typeToString() const;

  private:
	const TokenType _type;
	const std::string _string;
};

std::ostream &operator<<(std::ostream &os, Token &token);
std::ostringstream &operator<<(std::ostringstream &os, const Token &token);
#endif
