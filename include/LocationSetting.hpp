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

class LocationSetting
{
  public:
	LocationSetting();
	~LocationSetting();
	LocationSetting(const LocationSetting &src) = delete;
	LocationSetting &operator=(const LocationSetting &src) = delete;

	const std::string &
	getLocationSetting(const LocationSettingOption setting) const;
	void setLocationSetting(const LocationSettingOption key,
							const std::string &value);

  private:
	std::unordered_map<LocationSettingOption, std::string> _setting;
};
#endif // !LOCATIONSETTING_HPP
