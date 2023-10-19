#ifndef LOCATIONSETTING_HPP
#define LOCATIONSETTING_HPP

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
	LocationSettings(const LocationSettings &src) = delete;
	LocationSettings &operator=(const LocationSettings &src) = delete;

	const std::string &getLocationSetting(LocationSettingOption setting) const;
	void setLocationSetting(LocationSettingOption key,
							const std::string &value);

  private:
	std::unordered_map<LocationSettingOption, std::string> _setting;
};
#endif // !LOCATIONSETTING_HPP
