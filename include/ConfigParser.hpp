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

enum class ServerSettingOption
{
	Port,
	Host,
	ServerName,
	ClientMaxBodySize,
	ErrorPages,
};

enum class LocationSettingOption
{
	Prefix,
	Root,
	Index,
	DirectoryListing,
	AllowMethods,
	CgiPass,
};

struct LocationBlock
{
	std::map<LocationSettingOption, std::string> settings;
};

struct ServerBlock
{
	std::map<ServerSettingOption, std::string> settings;
	std::vector<LocationBlock> location_blocks;
};

class ConfigParser
{
  private:
	std::string &_configfile_path;
	std::unordered_map<GlobalSettingOption, std::string> _global_settings;
	std::vector<ServerBlock> _server_blocks;

	std::vector<Token> _tokens;

	void tokenizeConfigfile(std::ifstream &stream);
	void TokenValidator(void);

	void TokenParser(void);
	void ParseGlobalBlock(size_t &i);
	void ParseServerBlock(size_t &i);

  public:
	ConfigParser(const std::string &file_path);

	ConfigParser() = delete;
	ConfigParser(const ConfigParser &src) = delete;
	ConfigParser &operator=(const ConfigParser &rhs) = delete;

	const std::string &
	getGlobalSettings(const GlobalSettingOption setting) const;
	const std::vector<ServerBlock> &getServerBlocks() const;
};

#endif
