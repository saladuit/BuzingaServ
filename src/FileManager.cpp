#include <iostream>
#include <fstream>
#include <string>
#include "../include/Logger.hpp"
#include "../include/FileManager.hpp"
#include <filesystem>
#include <Logger.hpp>

namespace fs = std::filesystem;

FileManager::FileManager(): _status_code(0) {
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
			_status_code = 401;
			return ;
		}
		while (std::getline(file, line)) 
			_content += line + "\n";
		_status_code = 200;
	}
	else
		_status_code = 404;
	logger.log(DEBUG, "_status_code in the manageGet method is: %", _status_code);
}

// bool	save_data(const std::string post_message, const std::string content, 
// 			const std::string content_type)
// {
// 	std::ifstream	image_content(content);
// 	std::string		line;

// 	if (!image_content)
// 	{
// 		std::cerr << "Error opening files." << std::endl;
// 		return (false);
// 	}
// 	// std::cout << "data/" + content_type + "/" + post_message + "\n";
// 	std::ofstream	new_image("data/" + content_type + "/" + post_message);
// 	while (std::getline(image_content, line)) {
//     	new_image << line << '\n';
//     }
// 	return (true);
// }

void	FileManager::managePost(const std::string& filename, const std::string& body) {
	Logger &logger = Logger::getInstance();
	std::ofstream	newFile("data/upload/" + filename);

	logger.log(DEBUG, "managePost method is called");

	if (!newFile) {
		logger.log(ERROR, "Error opening file.");
		_status_code = 400;
    }
	else {
		newFile << body;
		_status_code = 201;
	}
	logger.log(DEBUG, "_status_code in the managePost method is: %", _status_code);
}

void	FileManager::manageDelete(const std::string& filename) {
	Logger  		&logger = Logger::getInstance();
	const std::string file_path("data/www/"+filename);
	std::ifstream		inputFile(file_path);

	logger.log(DEBUG, "manageDelete method is called");
	logger.log(DEBUG, "inputFile: " + file_path);

	if (!inputFile.is_open()) {
		logger.log(WARNING, "404 NOT FOUND");
		_status_code = 404;
	}
	else if (std::remove(file_path.c_str()) != 0) {
		logger.log(WARNING, "FORBIDDEN");
		_status_code = 403;
	}
	else {
		logger.log(INFO, "NO CONTENT");
		_status_code = 204;
	}
	logger.log(DEBUG, "_status_code in the manageDelete method is: %", _status_code);
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
	logger.log(DEBUG, "_status_code in the manage method is: %", _status_code);
}

const std::string& FileManager::getContent() const
{
	return (_content);
}

const int& FileManager::getStatusCode() const
{
	return (_status_code);
}
