#include <iostream>
#include <fstream>
#include <string>
#include "../include/Logger.hpp"
#include "../include/FileManager.hpp"
#include <filesystem>
#include <Logger.hpp>

namespace fs = std::filesystem;

FileManager::FileManager(): _statusCode(0) {
}

FileManager::~FileManager() {
}

void	FileManager::manageGet(const std::string& filename) {
	Logger  		&logger = Logger::getInstance();
	std::ifstream	file(filename);
	std::string 	line;

	logger.log(DEBUG, "manageGet method is called");

	if (fs::exists(filename)) 
	{
		if (!file.is_open()) 
		{
		_statusCode = 401;
		return ;
		}
		while (std::getline(file, line)) 
			_content += line + "\n";
		_statusCode = 200;
	}
	else
		_statusCode = 404;
	logger.log(DEBUG, "_statusCode in the manageGet method is: ", _statusCode);
}

void	FileManager::managePost(const std::string& filename, const std::string& body) {
	Logger &logger = Logger::getInstance();
	std::ofstream	newFile(filename);

	logger.log(DEBUG, "managePost method is called");

	if (!newFile) {
		logger.log(ERROR, "Error opening file.");
		_statusCode = 400;
    }
	else {
		newFile << body;
		_statusCode = 201;
	}
	logger.log(DEBUG, "_statusCode in the managePost method is: ", _statusCode);
}

void	FileManager::manageDelete(const std::string& filename) {
	Logger  		&logger = Logger::getInstance();
	std::ifstream		inputFile(filename);

	logger.log(DEBUG, "manageDelete method is called");

	if (!inputFile)
		_statusCode = 404;
	else if (std::remove(filename.c_str()) != 0) 
		_statusCode = 403;
	else
		_statusCode = 204;
	logger.log(DEBUG, "_statusCode in the manageDelete method is: ", _statusCode);
}

void	FileManager::manage(HTTPMethod method, const std::string &filename, const std::string& body)
{
	Logger  		&logger = Logger::getInstance();
	logger.log(DEBUG, "manage method is called");
	if (method == HTTPMethod::GET)
		manageGet(filename);
	else if (method == HTTPMethod::POST)
		managePost(filename, body);
	else
		manageDelete(filename);
	logger.log(DEBUG, "_statusCode in the manage method is: ", _statusCode);
}

const std::string& FileManager::getContent() const
{
	return (_content);
}

const int& FileManager::getStatusCode() const
{
	return (_statusCode);
}
