#ifndef SERVERSETTING_HPP
#define SERVERSETTING_HPP

#include <LocationSetting.hpp>

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

class ServerSetting
{
  public:
	ServerSetting();
	~ServerSetting();
	ServerSetting(const ServerSetting &src) = delete;
	ServerSetting &operator=(const ServerSetting &src) = delete;

	const std::string &
	getServerSetting(const ServerSettingOption setting) const;
	void setServerSetting(const ServerSettingOption key,
						  const std::string &value);
	void add_location_setting(const LocationSetting &setting);

  private:
	std::unordered_map<ServerSettingOption, std::string> _server_setting;
	std::vector<LocationSetting> _location_settings;
};

#endif
