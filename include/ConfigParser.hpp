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

enum class GlobalSetting
{
	Threads,
	DefaultErrorPages,
	ReadSize,
	WriteSize,
};

enum class ServerSetting
{
	Port,
	Host,
	ServerName,
	ClientMaxBodySize,
	ErrorPages,
};

enum class LocationSetting
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
	std::map<LocationSetting, std::string> settings;
};

struct ServerBlock
{
	std::map<ServerSetting, std::string> settings;
	std::vector<LocationBlock> location_blocks;
};

class ConfigParser
{
  private:
	std::ifstream _config_file;
	std::unordered_map<GlobalSetting, std::string> _global_settings;
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

	const std::string &getGlobalSettings(const GlobalSetting setting) const;
	const std::vector<ServerBlock> &getServerBlocks() const;
};

#endif
