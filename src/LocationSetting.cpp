
#include <LocationSetting.hpp>
#include <Logger.hpp>

#include <string>
#include <unordered_map>
#include <vector>

LocationSetting::LocationSetting() : _setting()
{
}

LocationSetting::~LocationSetting()
{
}

LocationSetting::LocationSetting(const LocationSetting &src)
{
	_setting = src._setting;
}

const std::string &
LocationSetting::getLocationSetting(const LocationSettingOption setting) const
{
	return (_setting.at(setting));
}

void LocationSetting::setLocationSetting(const LocationSettingOption key,
										 const std::string &value)
{
	_setting.emplace(key, value);
}
