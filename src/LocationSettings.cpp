
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

LocationSettings::LocationSettings()
	: _directory(), _alias(), _index(), _allowed_methods(), _cgi_path(),
	  _auto_index()
{
}

LocationSettings::LocationSettings(const LocationSettings &rhs)
	: _directory(rhs._directory), _alias(rhs._alias), _index(rhs._index),
	  _allowed_methods(rhs._allowed_methods), _cgi_path(rhs._cgi_path),
	  _auto_index(rhs._auto_index)
{
}

LocationSettings::~LocationSettings()
{
}

LocationSettings::LocationSettings(std::vector<Token>::iterator &token)
	: _directory(), _alias(), _index(), _allowed_methods(), _cgi_path(),
	  _auto_index()
{
	_directory = token->getString();
	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{
		const Token key = *token;
		token++;

		while (token->getType() != TokenType::SEMICOLON)
		{
			if (key.getString() == "root")
				parseAlias(*token);
			else if (key.getString() == "index")
				parseIndex(*token);
			else if (key.getString() == "allowed_methods")
				parseAllowedMethods(*token);
			else if (key.getString() == "cgi_path")
				parseCgiPath(*token);
			else if (key.getString() == "return")
				parseReturn(*token);

			token++;
		}
		token++;
	}
}

void LocationSettings::parseAlias(const Token token)
{
	_alias.append(" " + token.getString());
}

void LocationSettings::parseIndex(const Token token)
{
	_index.append(" " + token.getString());
}

void LocationSettings::parseAllowedMethods(const Token token)
{
	_allowed_methods.append(" " + token.getString());
}

void LocationSettings::parseCgiPath(const Token token)
{
	_cgi_path.append(" " + token.getString());
}

void LocationSettings::parseReturn(const Token token)
{
	_return.append(" " + token.getString());
}

// Functionality:
//		getters:
const std::string &LocationSettings::getDir() const
{
	return (_directory);
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

//		setters:
void LocationSettings::setDir(const std::string &direcotry)
{
	_directory = direcotry;
}

// THIS IS PRINTING FUNCTION

void LocationSettings::printLocationSettings() const
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "\tLocation_ Instance: " + _directory);
	logger.log(DEBUG, "\t\tAlias:\t\t\t" + _alias);
	logger.log(DEBUG, "\t\tIndex:\t\t\t" + _index);
	logger.log(DEBUG, "\t\tAllowed_methods:\t" + _allowed_methods);
	logger.log(DEBUG, "\t\tCGI Path:\t\t" + _cgi_path);
	logger.log(DEBUG, "\t\tReturn:\t\t" + _return);
	logger.log(DEBUG,
			   "\t\tAutoIndex:\t\t" +
				   (_auto_index ? std::string(" ON") : std::string(" OFF")));
}
