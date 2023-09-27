#include "ConfigParser.hpp"
#include "Logger.hpp"
#include <fstream>
#include <sstream>

ConfigParser::ConfigParser(const std::string &file_path)
	: _file_path(file_path), _global_settings(), _server_blocks()
{
	readConfig();
}

const std::string &
ConfigParser::getGlobalSettings(const GlobalSetting setting) const
{
	return (_global_settings.at(setting));
}

const std::vector<ServerBlock> &ConfigParser::getServerBlocks() const
{
	return (_server_blocks);
}

std::string stripInlineComments(const std::string &line)
{
	auto commentPosition = line.find('#');
	if (commentPosition != std::string::npos)
		return (line.substr(0, commentPosition));
	return (line);
}

void ConfigParser::parseLocationBlock(std::istream &stream,
									  const std::string &prefix)
{
	Logger &logger = Logger::getInstance();
	logger.log(LogLevel::DEBUG, "Parsing location block");
	LocationBlock location_block;
	std::string line;

	logger.log(LogLevel::DEBUG, "Prefix: " + prefix);
	location_block.settings[LocationSetting::Prefix] = prefix;
	while (std::getline(stream, line))
	{
		line = stripInlineComments(line);
		if (isCommentOrEmpty(line))
			continue;
		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;
		if (first_word == "}")
			break;
		else if (first_word == "root")
		{
			std::string root;
			line_stream >> root;
			location_block.settings[LocationSetting::Root] = root;
			logger.log(LogLevel::DEBUG, "Root: " + root);
		}
		else if (first_word == "index")
		{
			std::string index;
			line_stream >> index;
			location_block.settings[LocationSetting::Index] = index;
			logger.log(LogLevel::DEBUG, "Index: " + index);
		}
		else if (first_word == "directory_listing")
		{
			std::string directory_listing;
			line_stream >> directory_listing;
			location_block.settings[LocationSetting::DirectoryListing] =
				directory_listing;
			logger.log(LogLevel::DEBUG,
					   "DirectoryListing: " + directory_listing);
		}
		else if (first_word == "allow_methods")
		{
			std::string allow_methods;
			line_stream >> allow_methods;
			location_block.settings[LocationSetting::AllowMethods] =
				allow_methods;
			logger.log(LogLevel::DEBUG, "AllowMethods: " + allow_methods);
		}
		else if (first_word == "cgi_pass")
		{
			std::string cgi_pass;
			line_stream >> cgi_pass;
			location_block.settings[LocationSetting::CgiPass] = cgi_pass;
			logger.log(LogLevel::DEBUG, "CgiPass: " + cgi_pass);
		}
		else
			logger.log(LogLevel::WARNING,
					   "Unkown location setting: " + first_word);
	}
}

void ConfigParser::parseServerBlock(std::istream &stream)
{
	Logger &logger = Logger::getInstance();
	logger.log(LogLevel::DEBUG, "Parsing server block");
	ServerBlock server_block;
	std::string line;
	while (std::getline(stream, line))
	{
		line = stripInlineComments(line);
		if (isCommentOrEmpty(line))
			continue;
		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;
		if (first_word == "}")
			break;
		else if (first_word == "port")
		{
			std::string port;
			line_stream >> port;
			server_block.settings[ServerSetting::Port] = port;
			logger.log(LogLevel::DEBUG, "Port: " + port);
		}
		else if (first_word == "host")
		{
			std::string host;
			line_stream >> host;
			server_block.settings[ServerSetting::Host] = host;
			logger.log(LogLevel::DEBUG, "Host: " + host);
		}
		else if (first_word == "server_name")
		{
			std::string server_name;
			line_stream >> server_name;
			server_block.settings[ServerSetting::ServerName] = server_name;
			logger.log(LogLevel::DEBUG, "ServerName: " + server_name);
		}
		else if (first_word == "client_max_body_size")
		{
			std::string client_max_body_size;
			line_stream >> client_max_body_size;
			server_block.settings[ServerSetting::ClientMaxBodySize] =
				client_max_body_size;
			logger.log(LogLevel::DEBUG,
					   "ClientMaxBodySize: " + client_max_body_size);
		}
		else if (first_word == "error_pages")
		{
			std::string error_pages;
			line_stream >> error_pages;
			server_block.settings[ServerSetting::ErrorPages] = error_pages;
			logger.log(LogLevel::DEBUG, "ErrorPages: " + error_pages);
		}
		else if (first_word == "location")
		{
			std::string prefix;
			line_stream >> prefix;
			parseLocationBlock(stream, prefix);
		}
		else
			logger.log(LogLevel::WARNING,
					   "Unkown location setting: " + first_word);
	}
	_server_blocks.push_back(server_block);
	logger.log(LogLevel::DEBUG, "Added server block\n");
}

void ConfigParser::parseGlobalBlock(std::istream &stream)
{
	Logger &logger = Logger::getInstance();
	logger.log(LogLevel::DEBUG, "Parsing global block");
	std::string line;
	while (std::getline(stream, line))
	{
		line = stripInlineComments(line);
		if (isCommentOrEmpty(line))
			continue;
		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;
		if (first_word == "}")
			break;
		if (first_word == "threads")
		{
			std::string threads;
			line_stream >> threads;
			_global_settings[GlobalSetting::Threads] = threads;
			logger.log(LogLevel::DEBUG, "Threads: " + threads);
		}
		else if (first_word == "default_error_pages")
		{
			std::string default_error_pages;
			line_stream >> default_error_pages;
			_global_settings[GlobalSetting::DefaultErrorPages] =
				default_error_pages;
			logger.log(LogLevel::DEBUG, "log_level: " + default_error_pages);
		}
		else if (first_word == "read_size")
		{
			std::string read_size;
			line_stream >> read_size;
			_global_settings[GlobalSetting::ReadSize] = read_size;
			logger.log(LogLevel::DEBUG, "read_size: " + read_size);
		}
		else if (first_word == "write_size")
		{
			std::string write_size;
			line_stream >> write_size;
			_global_settings[GlobalSetting::ReadSize] = write_size;
			logger.log(LogLevel::DEBUG, "write_size: " + write_size);
		}
		else
			logger.log(LogLevel::WARNING,
					   "Unkown global setting: " + first_word);
	}
	logger.log(LogLevel::DEBUG, "Added global block\n");
}

bool ConfigParser::isCommentOrEmpty(const std::string &line)
{
	return (line.empty() || line[0] == '#');
}

void ConfigParser::readConfigFile(std::istream &config_file)
{
	Logger &logger = Logger::getInstance();
	std::string line;

	while (std::getline(config_file, line))
	{
		if (isCommentOrEmpty(line))
			continue;

		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;

		if (first_word == "server")
			parseServerBlock(config_file);
		else if (first_word == "global")
			parseGlobalBlock(config_file);
		else
			logger.log(LogLevel::WARNING, "Unkown block type: " + first_word);
	}
}

std::ifstream ConfigParser::openConfigFile(const std::string &file_path)
{
	Logger &logger = Logger::getInstance();
	std::ifstream config_file(file_path);

	if (!config_file.is_open())
	{
		logger.log(LogLevel::FATAL, "Could not open config file: " + file_path);
		throw std::runtime_error("Config file could not be opened");
	}

	logger.log(LogLevel::INFO, "Opened config file: " + file_path);
	return (config_file);
}

void ConfigParser::readConfig()
{
	std::ifstream config_file = openConfigFile(_file_path);
	readConfigFile(config_file);
	config_file.close();
}
