
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// this prototype is here so the constructor can use it;
bool validateDirectory(const Token &path);

LocationSettings::LocationSettings()
	: _directory(), _root(), _index(), _allowed_methods(), _auto_index()
{
}

LocationSettings::LocationSettings(const LocationSettings &rhs)
	: _directory(rhs._directory), _root(rhs._root), _index(rhs._index),
	  _allowed_methods(rhs._allowed_methods), _auto_index(rhs._auto_index)
{
}

LocationSettings::~LocationSettings()
{
}

LocationSettings::LocationSettings(std::vector<Token>::iterator &token)
	: _directory(), _root(), _index(), _allowed_methods(), _auto_index()
{
	if (token->getString() != "location")
		throw std::runtime_error(
			"unrecognised token in Configfile at token: " +
			token->getString()); // TODO: Make unrecognised token exception

	token++;
	//	if (!validateDirectory(token->getString()))
	//		throw std::runtime_error("location block contains invalid direcotry:
	//"
	//+ 								 token->getString());
	_directory = token->getString();
	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{

		token++;
	}
}

// bool validateDirectory(const Token &path)
//{
//	return (false);
// }

// Functionality:
//		getters:
const std::string &LocationSettings::getDir() const
{
	return (_directory);
}

const std::string &LocationSettings::getRoot() const
{
	return (_root);
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

//		setters:
void LocationSettings::setDir(const std::string &direcotry)
{
	_directory = direcotry;
}

// THIS IS PRINTING FUNCTION

void LocationSettings::printLocationSettings() const
{

	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "\tLocation_ Instance:");
	logger.log(DEBUG, "\t\tRequest_target:\t" + _directory);
	logger.log(DEBUG, "\t\tRoot:\t\t" + _root);
	logger.log(DEBUG, "\t\tIndex:\t\t" + _index);
	logger.log(DEBUG, "\t\tAllowed_methods:\t" + _allowed_methods);
	logger.log(DEBUG, "\t\tAutoIndex:\t" + (_auto_index ? std::string(" ON")
														: std::string(" OFF")));
}
