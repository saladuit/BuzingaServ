#include "ClientState.hpp"
#include <ClientException.hpp>
#include <FileManager.hpp>
#
#include <Logger.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::openFile(const std::string &request_target_path)
{
	if (!std::filesystem::exists(request_target_path))
		throw ClientException(StatusCode::NotFound);
	_request_target.open(request_target_path);
	if (!_request_target.is_open()) // TODO: check if this is right
		throw ClientException(StatusCode::Forbidden);
}

ClientState FileManager::manageGet(const std::string &request_target_path)
{
	Logger &logger = Logger::getInstance();
	char buffer[BUFFER_SIZE];

	if (!_request_target.is_open())
		openFile(request_target_path);
	logger.log(DEBUG, "manageGet method is called");
	_request_target.read(buffer, BUFFER_SIZE); // TODO: Add internal exception
	if (_request_target.fail())
		throw ClientException(StatusCode::InternalServerError);
	_content += std::string(buffer);
	if (_request_target.eof())
		return (ClientState::Sending);
	return (ClientState::Loading);
}

ClientState FileManager::managePost(const std::string &filename,
									const std::string &body)
{
	Logger &logger = Logger::getInstance();
	std::ofstream newFile("data/upload/" + filename);

	logger.log(DEBUG, "managePost method is called");
	logger.log(DEBUG, "body is: " + body);

	if (!newFile)
	{
		logger.log(WARNING, "400 BAD REQUEST");
		_status_code = 400;
	}
	else
	{
		newFile << body;
		logger.log(INFO, "201 CREATED");
		_status_code = 201;
	}
	logger.log(DEBUG, "_status_code in the managePost method is: %",
			   _status_code);
}

// again, here below hard code, handle with input config file?
//	const std::string file_path("data/www/"+filename);
ClientState FileManager::manageDelete(const std::string &filename)
{
	Logger &logger = Logger::getInstance();
	const std::string file_path("data/www/" + filename);
	std::ifstream inputFile(file_path);

	logger.log(DEBUG, "manageDelete method is called");
	logger.log(DEBUG, "inputFile: " + file_path);

	if (!inputFile.is_open())
	{
		logger.log(WARNING, "404 NOT FOUND");
		_status_code = 404;
	}
	else if (std::remove(file_path.c_str()) != 0)
	{
		logger.log(WARNING, "403 FORBIDDEN");
		_status_code = 403;
	}
	else
	{
		logger.log(INFO, "204 NO CONTENT");
		_status_code = 204;
	}
	logger.log(DEBUG, "_status_code in the manageDelete method is: %",
			   _status_code);
}

ClientState FileManager::manage(HTTPMethod method,
								const std::string &request_target_path,
								const std::string &body)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "manage method is called");
	if (method == HTTPMethod::GET)
		manageGet(request_target_path);
	else if (method == HTTPMethod::POST)
		managePost(request_target_path, body);
	else
		manageDelete(request_target_path);
}

const std::string &FileManager::getContent() const
{
	return (_content);
}
