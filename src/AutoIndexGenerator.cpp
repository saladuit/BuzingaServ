
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

	for (std::string::iterator it = str.begin(); it != str.end() - 1; ++it)
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
			<th style =\"text-align:left\" >" + std::string(direnty->d_name) + "</th>\n\
			<th style =\"text-align:left\" >" + ((statreturn != -1) ? getStatDateLastModified(filestat) : "UNKOWN") + "</th>\n\
			<th style =\"text-align:left\" >" + ((statreturn != -1) ? getStatSize(filestat) : "UNKNOWN") + "</th>\n\
		</tr>\n";
	}
	// clang-format on 
}

std::string AutoIndexGenerator::AutoIndexGenerator(const std::string dir,
												   const std::string uri)
{
	std::string response;

	// clang-format off
	response = "\
<!DOCTYPE html>\n\
<html>\n\
	<head>\n\
	\t<title> Index of ";
	response += uri;
	response += "</title>\n\
	</head>\n\
	<body>\n\
		<h1> Index of ";
	response += uri;
	response += "</h1>\n\
		<hr>\n\
		<table style = \"width:80%;font-size:15px\">\n\
		<tbody>\n\
		<tr>\n\
		\t<th style = \"text-align:left\"> <u>File Name</u></th>\n\
		\t<th style = \"text-align:left\"> <u>File Size</u></th>\n\
		\t<th style = \"text-align:left\"> <u>Last Modification</u></th>\n\
		<hr>\n\
		</tr>\n";

	getTableLines(response, dir);

	response += "\
		</tbody>\n\
		</table>\n\
	</body>\n";
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

	std::fstream return_fstream(filename, std::ios::in);
	std::remove(filename.c_str());

	return (return_fstream);
}
