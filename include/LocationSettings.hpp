#ifndef LOCATIONSETTING_HPP
#define LOCATIONSETTING_HPP

#include <Token.hpp>

#include <string>
#include <unordered_map>
#include <vector>

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
	const std::string &getDir() const;
	const std::string &getRoot() const;
	const std::string &getIndex() const;
	const std::string &getAllowedMethods() const;
	bool getAutoIndex() const;

	//		setters:
	void setDir(const std::string &path);

	// Printing:
	void printLocationSettings() const;

  private:
	std::string _directory;

	std::string _root;
	std::string _index;
	std::string _allowed_methods;
	bool _auto_index;
};
#endif // !LOCATIONSETTING_HPP
