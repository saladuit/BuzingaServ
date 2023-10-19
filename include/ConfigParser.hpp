#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <LocationSetting.hpp>
#include <ServerSetting.hpp>
#include <Tokenlist.hpp>

#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser
{
  private:
	const std::string _config_file_path;
	Tokenlist _tokenlist;
	std::vector<ServerSetting> _server_settings;

	std::stringstream OpenFile();

  public:
	ConfigParser(const std::string &file_path);
	~ConfigParser();

	ConfigParser() = delete;
	ConfigParser(const ConfigParser &src) = delete;
	ConfigParser &operator=(const ConfigParser &src) = delete;

	void ParseConfig();

	const std::vector<ServerSetting> &getServerSettings();
};

#endif
