#ifndef LOCATIONSETTING_HPP
#define LOCATIONSETTING_HPP

#include <Token.hpp>

#include <string>

// ENUM
#ifndef HTTP_METHOD_ENUM
#define HTTP_METHOD_ENUM
enum class HTTPMethod
{
	GET,
	POST,
	DELETE,
	UNKNOWN,
};
#endif

class LocationSettings
{
  public:
	LocationSettings();
	~LocationSettings();
	LocationSettings(std::vector<Token>::iterator &token);
	LocationSettings(const LocationSettings &rhs);
	LocationSettings &operator=(LocationSettings &rhs);

	// Functionality:
	//		getters:

	const std::string &getPath() const;
	const std::string &getAlias() const;
	const std::string &getIndex() const;
	const std::string &getAllowedMethods() const;
	const std::string &getReturn() const;
	bool getAutoIndex() const;

	//		resolves:

	const std::string resolveAlias(const std::string request_target) const;
	bool resolveMethod(const HTTPMethod method) const;

	// Printing:
	void printLocationSettings() const;

  private:
	std::string _path;

	std::string _alias;
	std::string _index;
	std::string _allowed_methods;
	std::string _cgi_path;
	std::string _return;
	bool _auto_index;

	void parseAlias(const Token token);
	void parseIndex(const Token token);
	void parseAutoIndex(const Token token);
	void parseAllowedMethods(const Token token);
	void parseCgiPath(const Token token);
	void parseReturn(const Token token);
};
#endif // !LOCATIONSETTING_HPP
