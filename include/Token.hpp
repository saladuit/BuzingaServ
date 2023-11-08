#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <sstream>
#include <vector>

#ifndef TOKENLIST_SPECIAL_CHAR
#define TOKENLIST_SPECIAL_CHAR "{};"
#endif

enum class TokenType
{
	SEMICOLON,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	PATH,
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
	void setType(TokenType);
	const std::string &getString() const;
	std::string typeToString() const;

  private:
	TokenType _type;
	const std::string _string;
};

#endif
