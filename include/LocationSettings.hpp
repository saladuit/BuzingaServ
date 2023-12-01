#ifndef LOCATIONSETTING_HPP
#define LOCATIONSETTING_HPP

#include <Token.hpp>

#include <string>

class LocationSettings
{
  public:
	LocationSettings();
	~LocationSettings();
	LocationSettings(std::vector<Token>::iterator &token);
	LocationSettings(const LocationSettings &rhs);
	LocationSettings &operator=(const LocationSettings &rhs) = delete;

	// Functionality:
	//		getters:
	const std::string &getRequestTarget() const;
	const std::string &getAlias() const;
	const std::string &getIndex() const;
	const std::string &getAllowedMethods() const;
	const std::string &getReturn() const;
	bool getAutoIndex() const;

	//		setters:
	void setDir(const std::string &path);

	// Printing:
	void printLocationSettings() const;

  private:
	std::string _requesttarget;

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
