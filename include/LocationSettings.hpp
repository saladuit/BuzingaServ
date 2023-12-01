#ifndef LOCATIONSETTING_HPP
#define LOCATIONSETTING_HPP

#include <Token.hpp>

#include <string>
#include <unordered_map>
#include <vector>

enum class LocationSettingOption
{
	Prefix,
	Root,
	Index,
	DirectoryListing,
	AllowedMethods,
	CgiPass,
};

class LocationSettings
{
  public:
	LocationSettings();
	~LocationSettings();
	LocationSettings(std::vector<Token>::iterator &token);
	LocationSettings(const LocationSettings &rhs);
	LocationSettings &operator=(const LocationSettings &rhs) = delete;

	const std::vector<std::string> &
	getValues(LocationSettingOption setting) const;
	void addValue(LocationSettingOption key, const std::string &value);

	const std::string &getPath() const;
	void setPath(const std::string &path);

	void printLocationSettings() const;

	std::string keyToString(LocationSettingOption Key) const;
	std::string valuesToString(LocationSettingOption Key) const;

  private:
	std::unordered_map<LocationSettingOption, std::vector<std::string>>
		_setting;
	std::string _path;

	LocationSettingOption identifyLocationSetting(const std::string &token);
};
#endif // !LOCATIONSETTING_HPP
