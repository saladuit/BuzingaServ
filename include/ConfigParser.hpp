#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "Token.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// these options are no longer required, Threads won't be implemented.
// DefaultErrorPages will be done in each ServerBlock, ReadSize/WriteSize will
// be set to 1024
/*
enum class GlobalSettingOption
{
	Threads,
	DefaultErrorPages,
	ReadSize,
	WriteSize,
};
*/

class ConfigParser
{
  private:
	const std::string &_config_file_path;
	std::vector<ServerBlock> _server_blocks;

	std::vector<Token> _tokens;

	void tokenizeConfigfile(std::ifstream &stream);

  public:
	ConfigParser(const std::string &file_path);

	ConfigParser() = delete;
	ConfigParser(const ConfigParser &src) = delete;
	ConfigParser &operator=(const ConfigParser &src) = delete;

	const std::vector<ServerBlock> &getServerBlocks() const;
};

#endif
