#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <fstream>
#include <map>
#include <string>
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
	std::map<GlobalSetting, std::string> _global_settings;
	std::vector<ServerBlock> _server_blocks;

	void parseBlock(std::ifstream &config_file, ServerBlock &current_server);
	void parseServerBlock(std::ifstream &stream);
	void parseGlobalBlock(std::ifstream &stream);
	void parseLocationBlock(std::ifstream &stream, const std::string &URI);

	std::vector<std::string> tokenizeConfigFile(std::ifstream &config_file);
	void trimComment(std::string &line);
	void readConfigFile(std::ifstream &config_file);
	bool isCommentOrEmpty(const std::string &line);
	void validateConfig();

  public:
	ConfigParser(const std::string &file_path);

	ConfigParser() = delete;
	ConfigParser(const ConfigParser &src) = delete;
	ConfigParser &operator=(const ConfigParser &rhs) = delete;

	const std::string &getGlobalSettings(const GlobalSetting setting) const;
	const std::vector<ServerBlock> &getServerBlocks() const;

	void readConfig();
};

#endif
