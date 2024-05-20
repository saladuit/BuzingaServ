
#include <HTTPRequest.hpp>
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <ServerSettings.hpp>
#include <SystemException.hpp>
#include <Token.hpp>

#include <arpa/inet.h>
#include <netdb.h>
#include <regex>
#include <stdexcept>
#include <string>

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

ServerSettings &ServerSettings::operator=(const ServerSettings &rhs)
{
	if (this == &rhs)
		return (*this);
	_listen = rhs._listen;
	_server_name = rhs._server_name;
	_error_dir = rhs._error_dir;
	_client_max_body_size = rhs._client_max_body_size;
	_location_settings = rhs._location_settings;
	return (*this);
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

const std::string convertHost(const std::string &str)
{
	Logger &logger = Logger::getInstance();

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *res = NULL;
	int ret;

	if ((ret = getaddrinfo(str.c_str(), "http", &hints, &res)) != 0)
	{
		logger.log(WARNING,
				   "Unable to get IPv4: " + std::string(gai_strerror(ret)));
	}
	void *addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
	char ipstr[INET_ADDRSTRLEN];
	inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
	freeaddrinfo(res);
	return (std::string(ipstr));
}

const std::string validateListen(const std::string &str)
{
	size_t pos = str.find_first_of(":");
	if (pos == std::string::npos || pos != str.find_last_of(":"))
		throw std::runtime_error("Parsing Error: invalid value for listen");

	const std::string ip = str.substr(0, pos);
	const std::string port = str.substr(pos + 1, std::string::npos);

	try
	{
		int port_ = std::stoi(port);
		if (port_ < 1 || port_ > 65535)
			throw std::exception();
	}
	catch (std::exception &e)
	{
		throw std::runtime_error("Parsing Error: found invalid PORT in listen");
	}
	return (convertHost(ip) + ":" + port);
}

void ServerSettings::parseListen(const Token value)
{
	Logger &logger = Logger::getInstance();

	if (!_listen.empty())
		logger.log(WARNING, "ConfigParser: redefining listen");
	_listen = validateListen(value.getString());
}

void ServerSettings::parseServerName(const Token value)
{
	_server_name.append(" " + value.getString());
}

void ServerSettings::parseErrorDir(const Token value)
{
	Logger &logger = Logger::getInstance();

	if (!_error_dir.empty())
		logger.log(WARNING, "ConfigParser: redefining error_dir");
	_error_dir = value.getString();
}

void ServerSettings::parseClientMaxBodySize(const Token value)
{
	Logger &logger = Logger::getInstance();

	const std::regex rgx_pat = std::regex("^\\d{1,3}[KM]?$");

	std::sregex_iterator it(value.getString().begin(), value.getString().end(),
							rgx_pat);
	std::sregex_iterator end;

	if (std::distance(it, end) == 0)
	{
		logger.log(FATAL, "ConfigParser: clientmaxbodysize inpropperly "
						  "formated: \"d{1,3}[MK]?\"");
		throw std::runtime_error(
			"ConfigParser: invalid value for clientmaxbodysize");
	}

	if (!_client_max_body_size.empty())
		logger.log(WARNING, "ConfigParser: redefining clientmaxbodysize");

	_client_max_body_size = it->str();
}

void ServerSettings::addValueToServerSettings(
	const Token &key, std::vector<Token>::iterator &value)
{
	Logger &logger = Logger::getInstance();

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
		else
			logger.log(WARNING,
					   "ServerSettings: unknown KEY token: " + key.getString());
		value++;
	}
}

// Functionality:
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

// Funcion: find the longest possible locationblock that fits the
// request_target. request_target will be stripped from it's trailing input.
// (line 3) and expects LocationBlock requesttarget to always start and end with
// a '/'
//
// EXAMPLES:
//
//	server {
//	location / {}
//	location /images/ {}
//	location /images/png/ {}
//	}
//
// /image				=> /
// /some/example.jpg	=> /
// /images				=> /
// /images/				=> /images/
// /images/jpg/			=> /images/
// /images/png/			=> /images/png/
// /png/images/			=> /
//

const LocationSettings &
ServerSettings::resolveLocation(const std::string &request_target) const
{
	Logger &logger = Logger::getInstance();
	const LocationSettings *ret = nullptr;
	std::string searched = request_target.substr(0, request_target.find("?"));

	logger.log(DEBUG, "resolveLocation: reques:\t" + request_target);
	logger.log(DEBUG, "resolveLocation: search:\t" + request_target);
	for (const auto &instance : _location_settings)
	{
		instance.printLocationSettings();
		const size_t pos = request_target.find(instance.getPath());

		if (pos != 0)
			continue;
		if (ret == nullptr)
			ret = &instance;
		else if (instance.getPath().length() > ret->getPath().length())
			ret = &instance;
	}
	if (ret == nullptr)
		throw std::logic_error("Couldn't resolve Location in server: " +
							   _server_name);

	logger.log(DEBUG, "resolveLocation: found Block: " + ret->getPath());
	return (*ret);
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
	logger.log(DEBUG, "\t_ErrorDir: " + _error_dir);
	logger.log(DEBUG, "\t_ClientMaxBodySize: " + _client_max_body_size);

	for (auto &location_instance : _location_settings)
	{
		location_instance.printLocationSettings();
	}
	logger.log(DEBUG, "\n");

	// We can go over the different strings by using Getline
	//
	//	std::stringstream ss(getListen());
	//
	//	for (; std::getline(ss, option, ' ');)
	//		logger.log(DEBUG, "\t\t" + option);

	return;
}
