
#include <Logger.hpp>
#include <ServerSettings.hpp>
#include <Token.hpp>

#include <string>
#include <unordered_map>
#include <vector>

ServerSettings::ServerSettings() : _server_setting(), _location_settings()
{
}

ServerSettings::~ServerSettings()
{
}

// TODO: void addLocationSetting(LocationSettings settings);

const std::string &
ServerSettings::getServerSetting(ServerSettingOption setting) const
{
	return (_server_setting.at(setting)); // TODO: throw exception for something
}

void ServerSettings::setServerSetting(ServerSettingOption key,
									  const std::string &value)
{
	_server_setting.emplace(key, value);
}
