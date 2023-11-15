
#include <HTTPRequest.hpp>
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>
#include <SystemException.hpp>
#include <Token.hpp>

#include <array>
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

void ServerSettings::parseListen(const Token value)
{
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

const LocationSettings *
ServerSettings::resolveLocation(const std::string &request_target,
								HTTPMethod input_method)
{
	for (auto &location_instance : _location_settings)
	{
		if (location_instance.getDir() != request_target)
			continue;
		std::stringstream ss(location_instance.getAllowedMethods());
		std::string option;
		for (; std::getline(ss, option, ' ');)
			if (option == methodToString(input_method))
				return (&location_instance);
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
