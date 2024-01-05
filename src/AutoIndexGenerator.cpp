
#include "Logger.hpp"
#include <AutoIndexGenerator.hpp>

#include <fstream>
#include <string>

std::string excludeSpecialCharacters(std::string str)
{
	std::string result;

	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (*it == '/')
			result += "_";
		else
			result += *it;
	}
	return (result);
}

std::string AutoIndexGenerator::AutoIndexGenerator(const std::string dir)
{
	return (dir);
}

std::fstream AutoIndexGenerator::OpenAutoIndex(std::string directory,
											   const std::string uri)
{
	(void)uri;
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "OpenAutoIndex method is called:");
	directory = excludeSpecialCharacters(directory);

	const std::string filename = "/tmp/" + directory + ".autoindex";

	std::fstream autoindex_fstream(filename.c_str(), std::fstream::out);
	if (!autoindex_fstream.is_open())
		logger.log(DEBUG, filename + " isn't opened.");

	logger.log(DEBUG, AutoIndexGenerator(directory));
	autoindex_fstream.close();

	exit(1);
	std::fstream return_fstream(filename, std::ios::in);
	std::remove(filename.c_str());

	return (return_fstream);
}
