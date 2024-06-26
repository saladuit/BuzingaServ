#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <LocationSettings.hpp>
#include <ServerSettings.hpp>

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser
{
  private:
	const std::string _config_file_path;
	std::vector<ServerSettings> _server_settings;

	std::stringstream OpenFile();
	void syntax(std::vector<Token> token);

  public:
	ConfigParser(const std::string &file_path);
	~ConfigParser();

	ConfigParser() = delete;
	ConfigParser(const ConfigParser &src) = delete;
	ConfigParser &operator=(const ConfigParser &src) = delete;

	void ParseConfig();

	const std::vector<ServerSettings> &getServerSettings();
	std::vector<std::vector<ServerSettings>> sortServerSettings();
};

void tokenizeStream(std::stringstream sstream,
					std::vector<Token> &list); // Located @ Token.cpp

#endif
