#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

struct LocationBlock
{
	std::map<std::string, std::string> settings;
};

struct ServerBlock
{
	std::map<std::string, std::string> settings;
	std::vector<LocationBlock> location_blocks;
};

class ConfigParser
{
  private:
	std::map<std::string, std::string> _global_settings;
	std::vector<ServerBlock> _server_blocks;
	std::pair<std::string, std::string> parseLine(const std::string &line);
	void parseBlock(std::istream &stream, const std::string &blockName,
					ServerBlock &current_server);

  public:
	ConfigParser(const std::string &file_path);

	void readConfig();

	std::map<std::string, std::string> getGlobalSettings() const;
	std::vector<ServerBlock> getServerBlocks() const;
};

#endif
