
#include <ConfigParser.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

ConfigParser::ConfigParser(const std::string &file_path)
	: _config_file_path(file_path), _server_settings()
{
}

bool static SkipComments(std::ifstream &stream, std::string &token)
{
	size_t pos = token.find('#');
	if (pos != std::string::npos)
	{
		std::string filler;
		token.erase(pos);
		getline(stream, filler);
		return (true);
	}
	return (false);
}

std::ifstream ConfigParser::OpenFile(const std::string &file_path)
{
	if (std::filesystem::is_character_file(file_path))
		throw std::runtime_error(
			"OpenFile: given file_path is not a character_file: " + file_path);

	std::ifstream file_fd;
	file_fd.open(file_path, std::ios_base::in);
	if (!file_fd.is_open())
		throw std::runtime_error("OpenFile: given file_path can't be opened");

	return (file_fd);
}

std::vector<Token> ConfigParser::TokenizeStream(const std::ifstream stream)
{
	std::vector<Token> tokens;

	return (tokens);
}

void ConfigParser::ParseConfig()
{
	Logger &logger = Logger::getInstance();

	logger.log(INFO, "Parsing Config File");
	std::ifstream infile_stream = OpenFile(_config_file_path);
	logger.log(INFO, "file_path succesfully opened");

	std::vector<Token> tokens = TokenizeStream(infile_stream);
}
