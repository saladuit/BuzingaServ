#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

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
	const std::string _file_path;
	std::map<GlobalSetting, std::string> _global_settings;
	std::vector<ServerBlock> _server_blocks;

	void parseBlock(std::ifstream &config_file, ServerBlock &current_server);
	void parseServerBlock(std::istream &stream);
	void parseGlobalBlock(std::istream &stream);
	void parseLocationBlock(std::istream &stream, const std::string &URI);

	std::ifstream openConfigFile(const std::string &file_path);
	void readConfigFile(std::istream &config_file);
	bool isCommentOrEmpty(const std::string &line);
	void validateConfig();

  public:
	ConfigParser(const std::string &file_path);

	ConfigParser() = delete;
	ConfigParser(const ConfigParser &src) = delete;
	ConfigParser &operator=(const ConfigParser &rhs) = delete;

	void readConfig();

	const std::string &getGlobalSettings(const GlobalSetting setting) const;
	const std::vector<ServerBlock> &getServerBlocks() const;
};

#endif
