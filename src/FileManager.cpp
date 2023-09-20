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
// - inject the content of the file on the webpage or in the folder of the specified website
// - 
void	FileManager::managePost(const std::string& filename) {
	(void)filename;



	_statusCode = 201;
}

void	FileManager::manageDelete(const std::string& filename) {
	(void)filename;
	_statusCode = 204;
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