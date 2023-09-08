#include "ConfigParser.hpp"
#include "Logger.hpp"
#include <sstream>
#include <stdexcept>

ConfigParser::ConfigParser(const std::string &file_path)
	: _file_path(file_path), _global_settings(), _server_blocks()
{
}

std::pair<std::string, std::string>
ConfigParser::parseLine(const std::string &line)
{
	std::istringstream lineStream(line);
	std::string key, value;

	std::getline(lineStream, key, ' ');

	key.erase(0, key.find_first_not_of(" \t"));
	key.erase(key.find_last_not_of(" \t") + 1);

	std::getline(lineStream, value);

	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t") + 1);

	return {key, value};
}

void ConfigParser::parseBlock(std::istream &stream,
							  LocationBlock &current_location)
{
	std::string line;
	while (std::getline(stream, line))
	{
		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;

		if (first_word == "}")
			return;
		else if (!first_word.empty() && first_word[0] != '#')
		{
			std::pair<std::string, std::string> key_value = parseLine(line);
			current_location.settings[key_value.first] = key_value.second;
		}
	}
}

void ConfigParser::parseBlock(std::istream &stream, ServerBlock &current_server)
{
	std::string line;
	while (std::getline(stream, line))
	{
		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;

		if (first_word == "}")
			return;
		else if (first_word == "location")
		{
			std::string location_path;
			line_stream >> location_path;
			LocationBlock new_location_block;
			parseBlock(stream, new_location_block);
			current_server.location_blocks.push_back(new_location_block);
		}
		else if (!first_word.empty() && first_word[0] != '#')
		{
			std::pair<std::string, std::string> key_value = parseLine(line);
			current_server.settings[key_value.first] = key_value.second;
		}
	}
}

void ConfigParser::readConfig()
{
	Logger::log(LogLevel::LOG_INFO, "Reading config file: %s",
				_file_path.c_str());
	std::ifstream config_file(_file_path);
	if (!config_file.is_open())
		throw std::runtime_error("Could not open config file");

	std::string line;
	while (std::getline(config_file, line))
	{
		std::istringstream line_stream(line);
		std::string first_word;
		line_stream >> first_word;

		if (first_word == "server")
		{
			ServerBlock new_server_block;
			parseBlock(config_file, new_server_block);
			_server_blocks.push_back(new_server_block);
		}
		else if (first_word == "global" && first_word[0] != '#')
		{
			std::pair<std::string, std::string> key_value = parseLine(line);
			_global_settings[key_value.first] = key_value.second;
		}
	}
	config_file.close();
}

std::map<std::string, std::string> ConfigParser::getGlobalSettings() const
{
	return (_global_settings);
}

std::vector<ServerBlock> ConfigParser::getServerBlocks() const
{
	return (_server_blocks);
}

void ConfigParser::debug_print_server_blocks() const
{
	std::cout << "---- Debugging Server Blocks ----" << std::endl;

	for (const auto &serverBlock : _server_blocks)
	{
		std::cout << "Server Block:" << std::endl;

		for (const auto &setting : serverBlock.settings)
		{
			std::cout << "  " << setting.first << ": " << setting.second
					  << std::endl;
		}

		std::cout << "  Location Blocks:" << std::endl;
		for (const auto &locationBlock : serverBlock.location_blocks)
		{
			std::cout << "    Location Block:" << std::endl;
			for (const auto &setting : locationBlock.settings)
			{
				std::cout << "      " << setting.first << ": " << setting.second
						  << std::endl;
			}
		}
		std::cout << "-------------------------------" << std::endl;
	}
}
