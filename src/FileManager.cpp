#include <iostream>
#include <fstream>
#include <string>
#include "../include/Logger.hpp"
#include "../include/FileManager.hpp"
#include <filesystem>

namespace fs = std::filesystem;

FileManager::FileManager() {
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

// steps for managing the post method:
// - save the post file in file system
// - (optionally inject the post file in the html of the webpage)
void	FileManager::managePost(const std::string& filename) {
	(void)filename;
	// implement saveData.cpp here
	_statusCode = 201;
}

void	FileManager::manageDelete(const std::string& filename) {
	std::ifstream		inputFile(filename);

	if (!inputFile)
		_statusCode = 404;
	else if (std::remove(filename.c_str()) != 0) 
		_statusCode = 403;
	else
		_statusCode = 204;
	// std::cout << filename << std::endl;
}

void	FileManager::manage(HTTPMethod method, const std::string &filename)
{
	if (method == HTTPMethod::GET)
		manageGet(filename);
	else if (method == HTTPMethod::POST)
		managePost(filename);
	else
		manageDelete(filename);
}

std::string FileManager::getContent()
{
	return (_content);
}

int FileManager::getStatusCode()
{
	return (_statusCode);
}