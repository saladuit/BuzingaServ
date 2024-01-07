
#include "AutoIndexGenerator.hpp"
#include "Logger.hpp"

#include <dirent.h>
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

std::string AutoIndexGenerator::AutoIndexGenerator(const std::string dir,
												   const std::string uri)
{
	std::string response;

	(void)dir;
	(void)uri;
	/*
	<html>
	<head>
		<title> Index of DIR</ title>
	</ head>
	<body>
		<h1> Index of DIR</ h1>
		<table style = "width:80%;font-size:15px">
		<tbody>
		<tr>
			<th style = "text-align:left"> File Name</ th>
			<th style = "text-align:left"> Last Modification</ th>
			<th style = "text-align:left"> File Size</ th>
		</ tr><tr>
			<th style = "text-align:left">.</ th>
			< th style ="text-align:left" > 07 Jan</ th>
			< th style ="text-align:left" > 253 B</ th>
		</ tr>
		</ tbody>
		</ table>
	</ body>
	*/
	return (response);
}

std::fstream AutoIndexGenerator::OpenAutoIndex(std::string directory,
											   const std::string uri)
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "OpenAutoIndex method is called:");
	directory = excludeSpecialCharacters(directory);

	const std::string filename = "/tmp/" + directory + ".autoindex";

	std::fstream autoindex_fstream(filename.c_str(), std::fstream::out);
	if (!autoindex_fstream.is_open())
		logger.log(DEBUG, filename + " isn't opened.");

	logger.log(DEBUG, AutoIndexGenerator(directory, uri));
	autoindex_fstream.close();

	exit(1);
	std::fstream return_fstream(filename, std::ios::in);
	std::remove(filename.c_str());

	return (return_fstream);
}
