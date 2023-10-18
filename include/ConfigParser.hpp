#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <LocationSetting.hpp>
#include <ServerSetting.hpp>
#include <Token.hpp>

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser
{
  private:
	const std::string &_config_file_path;
	std::vector<ServerSetting> _server_settings;

	std::ifstream OpenFile(const std::string &file_path);
	std::vector<Token> TokenizeStream(const std::ifstream stream);

  public:
	ConfigParser(const std::string &file_path);
	~ConfigParser();

	ConfigParser() = delete;
	ConfigParser(const ConfigParser &src) = delete;
	ConfigParser &operator=(const ConfigParser &src) = delete;

	void ParseConfig();
};

#endif
