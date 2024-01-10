
#include "AutoIndexGenerator.hpp"
#include "Logger.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#include <filesystem>
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

const std::string getStatSize(struct stat &filestat)
{
	std::string str;
	char buf[100];
	time_t date = filestat.st_mtime;
	struct tm *time = localtime(&date);

	strftime(buf, sizeof(buf), "%d %m %y", time);
	str = std::string(buf);

	return (str);
}

const std::string getStatDateLastModified(struct stat &filestat)
{
	std::string str = std::to_string((intmax_t)filestat.st_size);
	return (str);
}

void getTableLines(std::string &response, const std::string dir)
{
	// clang-format off
	DIR *dirptr = opendir(dir.c_str());
	struct dirent *direnty;

	std::filesystem::path pwd = std::filesystem::current_path();
	short statreturn;
	std::string filepath;
	struct stat filestat;

	for (size_t i = 0 ; i < 2 ; i++)
		direnty = readdir(dirptr);
	while ((direnty = readdir(dirptr)) != NULL)
	{
		filepath = pwd.string() + "/" + dir + direnty->d_name;
		statreturn = stat(filepath.c_str(), &filestat);
		response += "\t\t<tr>\n\
			\t\t\t<th style =\"text-align:left\" >" + std::string(direnty->d_name) + "</th>\n\
			\t\t\t<th style =\"text-align:left\" >" + ((statreturn != -1) ? getStatDateLastModified(filestat) : "UNKOWN") + "</th>\n\
			\t\t\t<th style =\"text-align:left\" >" + ((statreturn != -1) ? getStatSize(filestat) : "UNKNOWN") + "</th>\n\
			\t\t</tr>";
	}
	// clang-format on 
}

std::string AutoIndexGenerator::AutoIndexGenerator(const std::string dir,
												   const std::string uri)
{
	std::string response;

	// clang-format off
	response = "<!DOCTYPE html>\n\
		<html>\n\
		\t<head>\n\
		\t\t<title> Index of ";
	response += uri;
	response += "</title>\n\
		\t</head>\n\
		\t<body>\n\
		\t\t<h1> Index of ";
	response += uri;
	response += "</h1>\n\
		\t\t<table style = \"width:80%;font-size:15px\">\n\
		\t\t<tbody>\n\
		\t\t<tr>\n\
		\t\t\t<th style = \"text-align:left\"> File Name</th>\n\
		\t\t\t<th style = \"text-align:left\"> File Size</th>\n\
		\t\t\t<th style = \"text-align:left\"> Last Modification</th>\n\
		\t\t</tr>\n";

	getTableLines(response, dir);

	response += "\
		\t\t</tbody>\n\
		\t\t</table>\n\
		\t</body>\n";
	// clang-format on
	return (response);
}

std::fstream AutoIndexGenerator::OpenAutoIndex(std::string directory,
											   const std::string uri)
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "OpenAutoIndex method is called:");
	const std::string escapeddir = excludeSpecialCharacters(directory);

	const std::string filename = "/tmp/" + escapeddir + ".autoindex";

	std::fstream autoindex_fstream(filename.c_str(), std::fstream::out);
	if (!autoindex_fstream.is_open())
		logger.log(DEBUG, filename + " isn't opened.");

	autoindex_fstream << AutoIndexGenerator(directory, uri);
	autoindex_fstream.close();

	exit(1);
	std::fstream return_fstream(filename, std::ios::in);
	std::remove(filename.c_str());

	return (return_fstream);
}
