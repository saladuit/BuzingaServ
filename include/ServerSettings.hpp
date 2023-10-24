#ifndef SERVERSETTING_HPP
#define SERVERSETTING_HPP

#include <LocationSettings.hpp>
#include <Token.hpp>

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
	Location,
};

class ServerSettings
{
  public:
	ServerSettings();
	ServerSettings(std::vector<Token>::iterator &token);
	~ServerSettings();
	ServerSettings(const ServerSettings &rhs);
	ServerSettings &operator=(const ServerSettings &rhs) = delete;

	const std::vector<std::string> &
	getServerSetting(ServerSettingOption setting) const;
	void setServerSetting(ServerSettingOption key, const std::string &value);

	ServerSettingOption identifyServerSetting(std::string token_string);
	// TODO: void addLocationSetting(LocationSettings settings);

	void printServerSettings() const;
	const std::string printServerSettingValue(ServerSettingOption Key) const;

  private:
	std::unordered_map<ServerSettingOption, std::vector<std::string>>
		_server_setting;
	std::vector<LocationSettings> _location_settings;

	// TODO: methods fucntion that can resolve if a read/write/delete can be
	// done on a certain location in the LocationSettings
};

#endif
