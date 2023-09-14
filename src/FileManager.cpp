#include <iostream>
#include <fstream>
#include <string>
#include "../include/Logger.hpp"
#include "../include/FileManager.hpp"

FileManager::FileManager() {
	_statusCode = 0;
}

FileManager::~FileManager() {
}

void FileManager::manage(const std::string& filename) {
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

std::string FileManager::getContent()
{
	return (_content);
}

int FileManager::getStatusCode()
{
	return (_statusCode);
}