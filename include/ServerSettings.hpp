#ifndef SERVERSETTING_HPP
#define SERVERSETTING_HPP

#include <LocationSettings.hpp>
#include <Token.hpp>

#include <string>

class ServerSettings
{
  public:
	ServerSettings();
	ServerSettings(std::vector<Token>::iterator &token);
	~ServerSettings();
	ServerSettings(const ServerSettings &rhs);
	ServerSettings &operator=(const ServerSettings &rhs);

	// Functionality:
	const LocationSettings &
	resolveLocation(const std::string &request_target) const;

	const std::string &getListen() const;
	const std::string &getServerName() const;
	const std::string &getRoot() const;
	const std::string &getErrorDir() const;
	const std::string &getClientMaxBodySize() const;

	// Printing:
	void printServerSettings() const;

  private:
	std::string _listen;
	std::string _server_name;
	std::string _root;
	std::string _error_dir;
	std::string _client_max_body_size;
	std::vector<LocationSettings> _location_settings;

	const LocationSettings &getRootLocationBlock() const;

	// Parsing:
	void addValueToServerSettings(const Token &key,
								  std::vector<Token>::iterator &value);
	void parseListen(const Token value);
	void parseServerName(const Token value);
	void parseRoot(const Token value);
	void parseErrorDir(const Token value);
	void parseClientMaxBodySize(const Token value);

	void validateBlock();
};

#endif
