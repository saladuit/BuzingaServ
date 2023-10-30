
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

LocationSettings::LocationSettings() : _setting(), _path()
{
}

LocationSettings::LocationSettings(std::vector<Token>::iterator &token)
{
	if (token->getString() != "location")
		throw std::runtime_error(
			"unrecognised token in Configfile at token: " +
			token->getString()); // TODO: Make unrecognised token exception

	std::vector<Token>::iterator tmp = token;
	token++;

	_path = token->getString();
	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{
		const LocationSettingOption key =
			identifyLocationSetting(token->getString());
		tmp = token;

		token++;
		while (token->getType() != TokenType::SEMICOLON)
		{
			if (_setting
					.try_emplace(key,
								 std::vector<std::string>{token->getString()})
					.second == false)
				_setting.at(key).push_back(token->getString());
			token++;
		}
		token++;
	}
	token++;
}

LocationSettings::~LocationSettings()
{
}

LocationSettings::LocationSettings(const LocationSettings &rhs)
	: _setting(rhs._setting), _path(rhs._path)
{
}

LocationSettingOption
LocationSettings::identifyLocationSetting(const std::string &token)
{
	if (token == "prefix")
		return (LocationSettingOption::Prefix);
	else if (token == "root")
		return (LocationSettingOption::Root);
	else if (token == "index")
		return (LocationSettingOption::Index);
	else if (token == "directory_listing")
		return (LocationSettingOption::DirectoryListing);
	else if (token == "allow_methods")
		return (LocationSettingOption::AllowedMethods);
	else if (token == "cgipass")
		return (LocationSettingOption::CgiPass);
	else
		throw std::runtime_error("Unknow Key Token in LocationSettings" +
								 token);
}

const std::vector<std::string> &
LocationSettings::getValues(LocationSettingOption setting) const
{
	return (_setting.at(setting));
}

void LocationSettings::addValue(LocationSettingOption key,
								const std::string &value)
{
	std::vector<std::string> vect = _setting.at(key);
	vect.emplace_back(value);
}

const std::string &LocationSettings::getPath() const
{
	return (_path);
}

void LocationSettings::setPath(const std::string &path)
{
	_path = path;
}

// THIS IS PRINTING FUNCTION

std::string LocationSettings::keyToString(LocationSettingOption Key) const
{
	switch (Key)
	{
	case (LocationSettingOption::Prefix):
		return ("Prefix");
	case (LocationSettingOption::Root):
		return ("Root");
	case (LocationSettingOption::Index):
		return ("Index");
	case (LocationSettingOption::DirectoryListing):
		return ("DirectoryListing");
	case (LocationSettingOption::AllowedMethods):
		return ("AllowedMethods");
	case (LocationSettingOption::CgiPass):
		return ("CgiPass");
	default:
		return ("__UNRECOGNISED KEY__");
	}
}

std::string LocationSettings::valuesToString(LocationSettingOption Key) const
{
	std::string ret;

	for (const std::string &it : getValues(Key))
		ret += it + " ";
	return (ret);
}

void LocationSettings::printLocationSettings() const
{
	Logger &logger = Logger::getInstance();
	size_t enum_size = sizeof(LocationSettingOption) /
					   sizeof(std::underlying_type<LocationSettingOption>);

	logger.log(DEBUG, "Path:\t" + _path);
	for (size_t i = 0; i <= enum_size + 1; i++)
	{
		logger.log(DEBUG,
				   "LocationSetting: Key:\t" +
					   keyToString(static_cast<LocationSettingOption>(i)));
		try
		{
			logger.log(DEBUG, "LocationSetting: Value:\t" +
								  valuesToString(
									  static_cast<LocationSettingOption>(i)));
		}
		catch (std::out_of_range &e)
		{
			logger.log(WARNING, "Missing option: " + std::string(e.what()));
		}
	}
}
