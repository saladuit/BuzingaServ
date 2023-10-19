#ifndef SERVERSETTING_HPP
#define SERVERSETTING_HPP

#include <LocationSettings.hpp>

#include <string>
#include <unordered_map>
#include <vector>

enum class ServerSettingOption
{
	Port,
	Host,
	ServerName,
	ClientMaxBodySize,
	ErrorPages,
};

class ServerSettings
{
  public:
	ServerSettings();
	~ServerSettings();
	ServerSettings(const ServerSettings &src) = delete;
	ServerSettings &operator=(const ServerSettings &src) = delete;

	const std::string &getServerSetting(ServerSettingOption setting) const;
	void setServerSetting(ServerSettingOption key, const std::string &value);

	// TODO: void addLocationSetting(LocationSettings settings);

  private:
	std::unordered_map<ServerSettingOption, std::string> _server_setting;
	std::vector<LocationSettings> _location_settings;

	// TODO: methods fucntion that can resolve if a read/write/delete can be
	// done on a certain location in the LocationSettings
};

#endif
