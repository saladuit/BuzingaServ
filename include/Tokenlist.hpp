#ifndef TOKENLIST_HPP
#define TOKENLIST_HPP

#include <Token.hpp>

#include <sstream>
#include <vector>

class Tokenlist
{
  public:
	Tokenlist();
	~Tokenlist();

	Tokenlist(const Tokenlist &rhs) = delete;
	const Tokenlist &operator=(const Tokenlist &rhs) = delete;

	std::vector<Token> &getTokens();

	void tokenizeSstream(const std::stringstream sstream);

  private:
	std::vector<Token> _tokens;

	void splitString(std::string input);
};

#endif // !TOKENIZE_HPP
