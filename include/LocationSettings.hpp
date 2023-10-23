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
	AllowMethods,
	CgiPass,
};

class LocationSettings
{
  public:
	LocationSettings();
	~LocationSettings();
	LocationSettings(std::vector<Token>::iterator token);
	LocationSettings(const LocationSettings &rhs);
	LocationSettings &operator=(const LocationSettings &rhs) = delete;

	const std::vector<std::string> &
	getLocationSetting(LocationSettingOption setting) const;
	void setLocationSetting(LocationSettingOption key,
							const std::string &value);
	const std::string getPath() const;
	void setPath(std::string path);

	LocationSettingOption identifyLocationSetting(const std::string token);

  private:
	std::unordered_map<LocationSettingOption, std::vector<std::string>>
		_setting;
	std::string _path;
};
#endif // !LOCATIONSETTING_HPP
