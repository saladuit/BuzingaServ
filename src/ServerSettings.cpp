
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>
#include <SystemException.hpp>
#include <Token.hpp>

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
	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{
		ServerSettingOption key = identifyServerSetting(token->getString());
		token++;
		if (key == ServerSettingOption::Location)
			_location_settings.emplace_back(LocationSettings(token));
		else
		{
			while (token->getType() != TokenType::SEMICOLON)
			{
				setServerSetting(key, token->getString());
				token++;
			}
		}
		token++;
	}
}

ServerSettings::~ServerSettings()
{
}

ServerSettingOption
ServerSettings::identifyServerSetting(std::string token_string)
{
	if (token_string == "port")
		return (ServerSettingOption::Port);
	else if (token_string == "host")
		return (ServerSettingOption::Host);
	else if (token_string == "servername")
		return (ServerSettingOption::ServerName);
	else if (token_string == "clientmaxbodysize")
		return (ServerSettingOption::ClientMaxBodySize);
	else if (token_string == "errorpages")
		return (ServerSettingOption::ErrorPages);
	else if (token_string == "location")
		return (ServerSettingOption::Location);
	else
		throw std::runtime_error("Unknow Key Token in ServerSetting" +
								 token_string);
}

// TODO: void addLocationSetting(LocationSettings settings);

const std::vector<std::string> &
ServerSettings::getServerSetting(ServerSettingOption setting) const
{
	return (_server_setting.at(setting)); // TODO: throw exception for something
}

void ServerSettings::setServerSetting(ServerSettingOption key,
									  const std::string &value)
{
	std::vector<std::string> &vect =
		_server_setting.at(key); // TODO: add to back of vector
	vect.emplace_back(value);
}
