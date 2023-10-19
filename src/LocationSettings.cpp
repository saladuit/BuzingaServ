
#include <LocationSettings.hpp>
#include <Logger.hpp>

#include <string>
#include <unordered_map>
#include <vector>

LocationSettings::LocationSettings() : _setting()
{
}

LocationSettings::~LocationSettings()
{
}

const std::string &
LocationSettings::getLocationSetting(LocationSettingOption setting) const
{
	return (_setting.at(setting));
}

void LocationSettings::setLocationSetting(LocationSettingOption key,
										  const std::string &value)
{
	_setting.emplace(key, value);
}
