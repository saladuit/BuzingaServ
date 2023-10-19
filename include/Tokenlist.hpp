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

	void tokenizeSstream(const std::stringstream sstream);

  private:
	std::vector<Token> _tokens;
};

#endif // !TOKENIZE_HPP
