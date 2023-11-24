
#include <HTTPRequest.hpp>
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>
#include <SystemException.hpp>
#include <Token.hpp>

#include <array>
#include <exception>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

ServerSettings::ServerSettings()
	: _listen(), _server_name(), _error_dir(), _client_max_body_size(),
	  _location_settings()
{
}

ServerSettings::ServerSettings(const ServerSettings &rhs)
	: _listen(rhs._listen), _server_name(rhs._server_name),
	  _error_dir(rhs._error_dir),
	  _client_max_body_size(rhs._client_max_body_size),
	  _location_settings(rhs._location_settings)
{
}

ServerSettings::~ServerSettings()
{
}

// Parsing:
// This constructor takes a vector of Tokens, goes over it and according to the
// assigned values will fill in the ServerSettings.

ServerSettings::ServerSettings(std::vector<Token>::iterator &token)
	: _listen(), _server_name(), _error_dir(), _client_max_body_size(),
	  _location_settings()
{
	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{
		const Token key = *token;
		token++;

		if (key.getString() == "location")
			_location_settings.emplace_back(LocationSettings(token));
		else
			addValueToServerSettings(key, token);
		token++;
	}
}

// TODO: value confirmation and validation should happen here or in syntax

void validateListen(const std::string &str)
{
	size_t pos = str.find_first_of(":");
	if (pos == std::string::npos || pos != str.find_last_of(":"))
		throw std::runtime_error("Parsing Error: invalid value for listen");

	const std::string ip = str.substr(0, pos);
	const std::string port = str.substr(pos + 1, std::string::npos);

	try
	{
		int port_ = std::stoi(port);
		if (port_ < 1 || port_ > 65535) //
			throw std::exception();
	}
	catch (std::exception &e)
	{
		throw std::runtime_error("Parsing Error: invalid port found in listen");
	}
}

void ServerSettings::parseListen(const Token value)
{
	validateListen(value.getString());
	_listen.append(" " + value.getString());
}

void ServerSettings::parseServerName(const Token value)
{
	_server_name.append(" " + value.getString());
}

void ServerSettings::parseErrorDir(const Token value)
{
	_error_dir = value.getString();
}

void ServerSettings::parseClientMaxBodySize(const Token value)
{
	_client_max_body_size = value.getString();
}

void ServerSettings::addValueToServerSettings(
	const Token &key, std::vector<Token>::iterator &value)
{
	while (value->getType() != TokenType::SEMICOLON)
	{
		if (key.getString() == "listen")
			parseListen(*value);
		else if (key.getString() == "server_name")
			parseServerName(*value);
		else if (key.getString() == "error_dir")
			parseErrorDir(*value);
		else if (key.getString() == "client_max_body_size")
			parseClientMaxBodySize(*value);
		value++;
	}
}

// Functionality:
const std::string
methodToString(HTTPMethod method) // TODO: change data_types in function
{
	switch (method)
	{
	case (HTTPMethod::GET):
		return ("GET");
	case (HTTPMethod::POST):
		return ("POST");
	case (HTTPMethod::DELETE):
		return ("DELETE");
	default:
		throw std::runtime_error("Unknown HTTPMethod");
	}
}

//		getters:
const std::string &ServerSettings::getListen() const
{
	return (_listen);
}

const std::string &ServerSettings::getServerName() const
{
	return (_server_name);
}

const std::string &ServerSettings::getErrorDir() const
{
	return (_error_dir);
}

const std::string &ServerSettings::getClientMaxBodySize() const
{
	return (_client_max_body_size);
}

const std::string extractTargetfromURI(const std::string URI)
{
	size_t pos_dot = URI.find_last_of(".");
	if (pos_dot == std::string::npos)
		return (URI);

	size_t pos_slash = URI.find_last_of("/");
	if (pos_slash == std::string::npos)
		throw std::runtime_error(
			"resolveLocation: URI has unfamiliar structure");
	if (pos_dot < pos_slash)
		return (URI);

	std::string target = URI.substr(0, pos_slash + 1);
	return (target);
}

// TODO: maybe throw and error/exception here so the calling function can handle
// it.
//
// URI has to request target with a '/' to show it's a direcotry. The URI will
// be trimmed to remove the file if there is one.
//
const LocationSettings *ServerSettings::resolveLocation(const std::string &URI)
{
	Logger &logger = Logger::getInstance();
	const std::string target = extractTargetfromURI(URI);

	logger.log(WARNING, "target: " + target);
	for (auto &location_instance : _location_settings)
	{
		if (location_instance.getRequestTarget() != target)
			continue;
	}
	return (NULL);
}

// Printing:

void ServerSettings::printServerSettings() const
{
	Logger &logger = Logger::getInstance();
	std::string option;

	logger.log(DEBUG, "ServerSettings:");

	// printing Class variables:
	logger.log(DEBUG, "\t_Listen:" + _listen);
	logger.log(DEBUG, "\t_ServerName:" + _server_name);
	logger.log(DEBUG, "\t_ErrorDir:" + _error_dir);
	logger.log(DEBUG, "\t_ClientMaxBodySize:" + _client_max_body_size);
	for (auto &location_instance : _location_settings)
	{
		logger.log(DEBUG, "\n");
		location_instance.printLocationSettings();
	}

	// We can go over the different strings by using Getline
	//
	//	logger.log(DEBUG, "\t_ClientMaxBodySize:");
	//
	//	std::stringstream ss(getListen());
	//
	//	for (; std::getline(ss, option, ' ');)
	//		logger.log(DEBUG, "\t\t" + option);

	return;
}
