#include <iostream>
#include <fstream>
#include <string>
#include "../include/Logger.hpp"
#include "../include/FileManager.hpp"
#include <filesystem>

namespace fs = std::filesystem;

FileManager::FileManager(): _statusCode(0) {
}

FileManager::~FileManager() {
}

void	FileManager::manageGet(const std::string& filename) {
	std::ifstream	file(filename);
	std::string line;

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
}

void	FileManager::managePost(const std::string& filename, const std::string& body) {
	Logger &logger = Logger::getInstance();
	std::ofstream	newFile(filename);

	if (!newFile) {
		logger.log(ERROR, "Error opening file.");
		_statusCode = 400;
    }
	else {
		newFile << body;
		_statusCode = 201;
	}
}

void	FileManager::manageDelete(const std::string& filename) {
	std::ifstream		inputFile(filename);

	if (!inputFile)
		_statusCode = 404;
	else if (std::remove(filename.c_str()) != 0) 
		_statusCode = 403;
	else
		_statusCode = 204;
}

void	FileManager::manage(HTTPMethod method, const std::string &filename, const std::string& body)
{
	if (method == HTTPMethod::GET)
		manageGet(filename);
	else if (method == HTTPMethod::POST)
		managePost(filename, body);
	else
		manageDelete(filename);
}

const std::string& FileManager::getContent() const
{
	return (_content);
}

const int& FileManager::getStatusCode() const
{
	return (_statusCode);
}
