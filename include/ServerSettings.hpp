#ifndef SERVERSETTING_HPP
#define SERVERSETTING_HPP

#include <HTTPRequest.hpp>
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
	Count,
};

class ServerSettings
{
  public:
	ServerSettings();
	ServerSettings(std::vector<Token>::iterator &token);
	~ServerSettings();
	ServerSettings(const ServerSettings &rhs);
	ServerSettings &operator=(const ServerSettings &rhs) = delete;

	const std::string &getValue(ServerSettingOption setting) const;
	void setValue(ServerSettingOption key, const std::string &value);

	ServerSettingOption identifyServerSetting(const std::string &token);
	// TODO: void addLocationSetting(LocationSettings settings);
	bool resolveLocation(const std::string &path, HTTPMethod method);

	void printServerSettings() const;

  private:
	std::unordered_map<ServerSettingOption, std::string> _server_setting;
	std::vector<LocationSettings> _location_settings;

	// TODO: methods fucntion that can resolve if a read/write/delete can be
	// done on a certain location in the LocationSettings
};

#endif