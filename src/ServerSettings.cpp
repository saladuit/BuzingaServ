
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>
#include <SystemException.hpp>
#include <Token.hpp>

#include <array>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

ServerSettings::ServerSettings() : _server_setting(), _location_settings()
{
}

ServerSettings::ServerSettings(const ServerSettings &rhs)
	: _server_setting(rhs._server_setting),
	  _location_settings(rhs._location_settings)
{
}

ServerSettings::ServerSettings(std::vector<Token>::iterator &token)
	: _server_setting(), _location_settings()
{
	if (token->getString() != "server")
		throw std::runtime_error(
			"unrecognised token in Configfile at token: " +
			token->getString()); // TODO: Make unrecognised token exception
								 // TODO: this needs to be moved to syntax

	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{
		ServerSettingOption key = identifyServerSetting(token->getString());
		if (key == ServerSettingOption::Location)
			_location_settings.emplace_back(LocationSettings(token));
		else
		{
			token++;
			setValue(key, token->getString());
			token += 2;
		}
	}
}

ServerSettings::~ServerSettings()
{
}

ServerSettingOption
ServerSettings::identifyServerSetting(const std::string &token)
{
	if (token == "port")
		return (ServerSettingOption::Port);
	else if (token == "host")
		return (ServerSettingOption::Host);
	else if (token == "server_name")
		return (ServerSettingOption::ServerName);
	else if (token == "client_max_body_size")
		return (ServerSettingOption::ClientMaxBodySize);
	else if (token == "error_pages")
		return (ServerSettingOption::ErrorPages);
	else if (token == "location")
		return (ServerSettingOption::Location);
	else
		throw std::runtime_error("Unknow Key Token in ServerSetting " + token);
	// TODO: this might need a costum exception
}

// TODO: void addLocationSetting(LocationSettings settings);

const std::string &ServerSettings::getValue(ServerSettingOption setting) const
{
	return (_server_setting.at(setting));
}

void ServerSettings::setValue(ServerSettingOption key, const std::string &value)
{
	_server_setting.emplace(key, value);
}

// THIS IS PRINTING FUNCTION

std::string keyToString(ServerSettingOption Key)
{
	switch (Key)
	{
	case (ServerSettingOption::Port):
		return ("Port");
	case (ServerSettingOption::Host):
		return ("Host");
	case (ServerSettingOption::ServerName):
		return ("ServerName");
	case (ServerSettingOption::ClientMaxBodySize):
		return ("ClientMaxBodySize");
	case (ServerSettingOption::ErrorPages):
		return ("ErrorPages");
	case (ServerSettingOption::Location):
		return ("Location");
	default:
		return ("OUT OF BOUND KEY");
	}
}

void ServerSettings::printServerSettings() const
{
	Logger &logger = Logger::getInstance();

	for (int i = static_cast<int>(ServerSettingOption::Port);
		 i < static_cast<int>(ServerSettingOption::Count); i++)
	{
		logger.log(DEBUG, "ServerSetting: Key:\t" +
							  keyToString(static_cast<ServerSettingOption>(i)));
		try
		{
			logger.log(DEBUG,
					   "ServerSetting: Value:\t" +
						   getValue(static_cast<ServerSettingOption>(i)));
		}
		catch (std::out_of_range &e)
		{
			logger.log(WARNING, "ServerSettings : Missing option: " +
									std::string(e.what()));
		}
	}

	for (const LocationSettings &loc : _location_settings)
	{
		logger.log(DEBUG, "LocationSettings: ");
		loc.printLocationSettings();
	}
}
