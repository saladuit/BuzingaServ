
#include <Logger.hpp>
#include <ServerSetting.hpp>

#include <string>
#include <unordered_map>
#include <vector>

ServerSetting::ServerSetting() : _server_setting(), _location_settings()
{
}

ServerSetting::~ServerSetting()
{
}

ServerSetting::ServerSetting(const ServerSetting &src)
{
	_server_setting = src._server_setting;
	_location_settings = src._location_settings;
}

const std::string &
ServerSetting::getServerSetting(const ServerSettingOption setting) const
{
	return (_server_setting.at(setting));
}
void ServerSetting::setServerSetting(const ServerSettingOption key,
									 const std::string &value)
{
	_server_setting.emplace(key, value);
}
