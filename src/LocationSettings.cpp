
#include <HTTPRequest.hpp>
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <stdexcept>
#include <string>

LocationSettings::LocationSettings()
	: _path(), _alias(), _index(), _allowed_methods(), _cgi(false), _redirect(),
	  _auto_index(false)
{
}

LocationSettings::LocationSettings(const LocationSettings &rhs)
	: _path(rhs._path), _alias(rhs._alias), _index(rhs._index),
	  _allowed_methods(rhs._allowed_methods), _cgi(rhs._cgi),
	  _redirect(rhs._redirect), _auto_index(rhs._auto_index)
{
}

LocationSettings &LocationSettings::operator=(const LocationSettings &rhs)
{
	if (this == &rhs)
		return (*this);

	_path = rhs._path;

	_alias = rhs._alias;
	_index = rhs._index;
	_allowed_methods = rhs._allowed_methods;
	_cgi = rhs._cgi;
	_redirect = rhs._redirect;
	_auto_index = rhs._auto_index;

	return (*this);
}

LocationSettings::~LocationSettings()
{
}

LocationSettings::LocationSettings(std::vector<Token>::iterator &token)
	: _cgi(false), _auto_index(false)
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
			{
				logger.log(WARNING, "LocationSettings: unknown KEY token: " +
										key.getString());
				break;
			}
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
	if (token.getString().front() == '/' && token.getString().back() == '/')
		_alias = token.getString();
	else
		throw std::runtime_error(
			"ConfigParser: invalid alias (/path/to/directory/) : [" +
			token.getString() + "] in block: " + getPath());
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
				   "ConfigParser: redefining cgi_path in locationblock: " +
					   _path);
	if (token.getString() == "on" || token.getString() == "ON")
		_cgi = true;
	else if (token.getString() == "off" || token.getString() == "OFF")
		_cgi = false;
	else
		throw std::runtime_error("ConfigParser: Unknown VALUE for cgi: " +
								 token.getString());
}

void LocationSettings::parseReturn(const Token token)
{
	Logger &logger = Logger::getInstance();

	if (!_redirect.empty())
		logger.log(WARNING,
				   "ConfigParser: redefining return in locationblock: " +
					   _path);
	_redirect = token.getString();
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

const bool &LocationSettings::getAutoIndex() const
{
	return (_auto_index);
}

const std::string &LocationSettings::getRedirect() const
{
	return (_redirect);
}

const bool &LocationSettings::getCGI() const
{
	return (_cgi);
}

const std::string MethodToString(HTTPMethod num)
{
	switch (num)
	{
	case (HTTPMethod::GET):
		return ("GET");
	case (HTTPMethod::POST):
		return ("POST");
	case (HTTPMethod::DELETE):
		return ("DELETE");
	default:
	{
		Logger &logger = Logger::getInstance();
		logger.log(WARNING, "LocationSettings MethodToString: unknown Method");
		return ("UNKOWNMETHOD");
	}
	}
}

// resolveMethod
bool LocationSettings::resolveMethod(const HTTPMethod method) const
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "resolveMethod: " + MethodToString(method) + " : " +
						  getAllowedMethods());
	if (getAllowedMethods().empty())
	{
		logger.log(WARNING,
				   "ResolveMethod: No HTTPMethod specified in Locationblock: " +
					   getPath());
		return (false);
	}

	std::stringstream ss(getAllowedMethods());
	std::string option;
	for (; std::getline(ss, option, ' ');)
	{
		if (option == MethodToString(method))
			return (true);
	}
	return (false);
}

// resolveAlias

const std::string LocationSettings::resolveAlias(const std::string inp) const
{
	Logger &logger = Logger::getInstance();

	std::string alias = getAlias();
	logger.log(DEBUG, "resolveAlias:\treques_target:\t" + inp);
	logger.log(DEBUG, "resolveAlias:\tAlias:\t\t" + alias);
	std::string result = inp.substr(_path.length());

	logger.log(DEBUG, "resolveAlias:\t\t\t" + alias + result);
	return (alias + result);
}

// THIS IS PRINTING FUNCTION

void LocationSettings::printLocationSettings() const
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "\tLocation Prefix:\t" + _path);
	logger.log(DEBUG, "\t\tAlias:\t\t\t" + _alias);
	logger.log(DEBUG, "\t\tIndex:\t\t\t" + _index);
	logger.log(DEBUG, "\t\tAllowed_methods:\t" + _allowed_methods);
	logger.log(DEBUG, "\t\tCGI:\t\t\t" +
						  (_cgi ? std::string(" ON") : std::string(" OFF")));
	logger.log(DEBUG, "\t\tRedirect:\t\t" + _redirect);
	logger.log(DEBUG,
			   "\t\tAutoIndex:\t\t" +
				   (_auto_index ? std::string(" ON") : std::string(" OFF")));
}
