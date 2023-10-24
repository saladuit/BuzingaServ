
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <string>
#include <unordered_map>
#include <vector>

LocationSettings::LocationSettings() : _setting(), _path()
{
}

LocationSettings::LocationSettings(std::vector<Token>::iterator token)
{
	if (token->getString() != "location")
		throw std::runtime_error(
			"unrecognised token in Configfile at token: " +
			token->getString()); // TODO: Make unrecognised token exception
	token++;
	_path = token->getString();
	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{
		const LocationSettingOption key =
			identifyLocationSetting(token->getString());
		token++;
		while (token->getType() != TokenType::SEMICOLON)
		{
			setLocationSetting(key, token->getString());
			token++;
		}
	}
}

LocationSettings::~LocationSettings()
{
}

LocationSettings::LocationSettings(const LocationSettings &rhs)
	: _setting(rhs._setting), _path(rhs._path)
{
}

LocationSettingOption
LocationSettings::identifyLocationSetting(const std::string token)
{
	if (token == "prefix")
		return (LocationSettingOption::Prefix);
	else if (token == "root")
		return (LocationSettingOption::Root);
	else if (token == "index")
		return (LocationSettingOption::Index);
	else if (token == "directorylisting")
		return (LocationSettingOption::DirectoryListing);
	else if (token == "allow_methods")
		return (LocationSettingOption::AllowMethods);
	else if (token == "cgipass")
		return (LocationSettingOption::CgiPass);
	else
		throw std::runtime_error("Unknow Key Token in LocatoinSettings" +
								 token);
}

const std::vector<std::string> &
LocationSettings::getLocationSetting(LocationSettingOption setting) const
{
	return (_setting.at(setting));
}

void LocationSettings::setLocationSetting(LocationSettingOption key,
										  const std::string &value)
{

	std::vector<std::string> &vect = _setting.at(key);
	vect.emplace_back(value);
}

// THIS IS PRINTING FUNCTION

// TODO: make this function
void LocationSettings::printLocationSettings() const
{
}
