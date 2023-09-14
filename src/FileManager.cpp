#include <iostream>
#include <fstream>
#include <string>
#include "../include/Logger.hpp"
#include "../include/FileManager.hpp"

FileManager::FileManager() {
}

FileManager::~FileManager() {
}

void	FileManager::manageGet(const std::string& filename) {
	std::ifstream	file(filename);
	std::string line;

	if (!file.is_open()) {
		_statusCode = 404;
		return ;
	}
	while (std::getline(file, line)) {
		_content += line + "\n";
	}
	_statusCode = 200;

}

void	FileManager::managePost(const std::string& filename) {
	(void)filename;
	_statusCode = 201;
}

void	FileManager::manageDelete(const std::string& filename) {
	(void)filename;
	_statusCode = 204;
}

void	FileManager::manage(int method, const std::string &filename)
{
	if (method == 0)
		manageGet(filename);
	else if (method == 1)
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