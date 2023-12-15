#ifndef SERVERSETTING_HPP
#define SERVERSETTING_HPP

#include <HTTPRequest.hpp>
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
	ServerSettings &operator=(const ServerSettings &rhs) = delete;

	// Functionality:
	const LocationSettings &resolveLocation(const std::string &URI) const;
	bool resolveServerName(const std::string &RequestHost);

	const std::string &getListen() const;
	const std::string &getServerName() const;
	const std::string &getErrorDir() const;
	const std::string &getClientMaxBodySize() const;

	// Printing:
	void printServerSettings() const;

  private:
	std::string _listen;
	std::string _server_name;
	std::string _error_dir;
	std::string _client_max_body_size;
	std::vector<LocationSettings> _location_settings;

	const LocationSettings &getRootLocationBlock() const;

	// Parsing:
	void addValueToServerSettings(const Token &key,
								  std::vector<Token>::iterator &value);
	void parseListen(const Token value);
	void parseServerName(const Token value);
	void parseErrorDir(const Token value);
	void parseClientMaxBodySize(const Token value);

	// TODO: methods fucntion that can resolve if a read/write/delete can be
	// done on a certain location in the LocationSettings
};

#endif
