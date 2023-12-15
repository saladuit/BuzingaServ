
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <stdexcept>
#include <string>

LocationSettings::LocationSettings()
	: _path(), _alias(), _index(), _allowed_methods(), _cgi_path(),
	  _auto_index()
{
}

LocationSettings::LocationSettings(const LocationSettings &rhs)
	: _path(rhs._path), _alias(rhs._alias), _index(rhs._index),
	  _allowed_methods(rhs._allowed_methods), _cgi_path(rhs._cgi_path),
	  _auto_index(rhs._auto_index)
{
}

LocationSettings &LocationSettings::operator=(LocationSettings &rhs)
{
	if (this == &rhs)
		return (*this);

	_path = rhs._path;

	_alias = rhs._alias;
	_index = rhs._index;
	_allowed_methods = rhs._allowed_methods;
	_cgi_path = rhs._cgi_path;
	_return = rhs._return;
	_auto_index = rhs._auto_index;

	return (*this);
}

LocationSettings::~LocationSettings()
{
}

LocationSettings::LocationSettings(std::vector<Token>::iterator &token)
	: _path(), _alias(), _index(), _allowed_methods(), _cgi_path(),
	  _auto_index()
{
	_path = token->getString();
	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{
		Logger &logger = Logger::getInstance();

		const Token key = *token;
		token++;

		while (token->getType() != TokenType::SEMICOLON)
		{
			if (key.getString() == "alias")
				parseAlias(*token);
			else if (key.getString() == "index")
				parseIndex(*token);
			else if (key.getString() == "autoindex")
				parseAutoIndex(*token);
			else if (key.getString() == "allowed_methods")
				parseAllowedMethods(*token);
			else if (key.getString() == "cgi")
				parseCgiPath(*token);
			else if (key.getString() == "return")
				parseReturn(*token);
			else
				logger.log(WARNING, "LocationSettings: unknown KEY token: " +
										key.getString());

			token++;
		}
		token++;
	}
}

void LocationSettings::parseAlias(const Token token)
{
	Logger &logger = Logger::getInstance();

	if (!_alias.empty())
		logger.log(WARNING,
				   "ConfigParser: redefining alias in locationblock: " + _path);
	_alias = token.getString();
}

void LocationSettings::parseIndex(const Token token)
{
	Logger &logger = Logger::getInstance();

	if (!_index.empty())
		logger.log(WARNING,
				   "ConfigParser: redefining index in locationblock: " + _path);
	_index = token.getString();
}

void LocationSettings::parseAutoIndex(const Token token)
{
	if (token.getString() == "on")
		_auto_index = true;
	else if (token.getString() == "off")
		_auto_index = false;
	else
		throw std::runtime_error("ConfigParser: Unknown VALUE for autoindex: " +
								 token.getString());
}

void LocationSettings::parseAllowedMethods(const Token token)
{
	if (token.getString() != "GET" && token.getString() != "POST" &&
		token.getString() != "DELETE")
		throw std::runtime_error(
			"ConfigParser: Unknown VALUE for allowed_methods: " +
			token.getString());
	_allowed_methods.append(" " + token.getString());
}

void LocationSettings::parseCgiPath(const Token token)
{
	Logger &logger = Logger::getInstance();

	if (!_index.empty())
		logger.log(WARNING,
				   "ConfigParser: redefining index in locationblock: " + _path);
	_cgi_path = token.getString();
}

void LocationSettings::parseReturn(const Token token)
{
	Logger &logger = Logger::getInstance();

	if (!_return.empty())
		logger.log(WARNING,
				   "ConfigParser: redefining return in locationblock: " +
					   _path);
	_return = token.getString();
}

// Functionality:
//		getters:
const std::string &LocationSettings::getPath() const
{
	return (_path);
}

const std::string &LocationSettings::getAlias() const
{
	return (_alias);
}

const std::string &LocationSettings::getIndex() const
{
	return (_index);
}

const std::string &LocationSettings::getAllowedMethods() const
{
	return (_allowed_methods);
}

bool LocationSettings::getAutoIndex() const
{
	return (_auto_index);
}

const std::string &LocationSettings::getReturn() const
{
	return (_return);
}

//

// THIS IS PRINTING FUNCTION

void LocationSettings::printLocationSettings() const
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "\tLocation Prefix:\t" + _path);
	logger.log(DEBUG, "\t\tAlias:\t\t\t" + _alias);
	logger.log(DEBUG, "\t\tIndex:\t\t\t" + _index);
	logger.log(DEBUG, "\t\tAllowed_methods:\t" + _allowed_methods);
	logger.log(DEBUG, "\t\tCGI Path:\t\t" + _cgi_path);
	logger.log(DEBUG, "\t\tReturn:\t\t" + _return);
	logger.log(DEBUG,
			   "\t\tAutoIndex:\t\t" +
				   (_auto_index ? std::string(" ON") : std::string(" OFF")));
}
