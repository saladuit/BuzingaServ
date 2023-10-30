#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <LocationSettings.hpp>
#include <ServerSettings.hpp>

#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser
{
  private:
	const std::string _config_file_path;
	std::vector<ServerSettings> _server_settings;

	std::stringstream OpenFile();

  public:
	ConfigParser(const std::string &file_path);
	~ConfigParser();

	ConfigParser() = delete;
	ConfigParser(const ConfigParser &src) = delete;
	ConfigParser &operator=(const ConfigParser &src) = delete;

	void ParseConfig();

	const std::vector<ServerSettings> &getServerSettings();
};

void tokenizeStream(std::stringstream sstream,
					std::vector<Token> &list); // Located @ Token.cpp

#endif
