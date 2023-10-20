#include <FileManager.hpp>
#include <Logger.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

FileManager::FileManager() : _status_code(0)
{
}

FileManager::~FileManager()
{
}

void FileManager::openFile(std::string &request_target)
{
	(void)request_target;
	/* _requested_file.open(request_target); // TODO throw exception if fail */
}

// the following line is hard code, need to fix this with input of the config
// file std::ifstream	file("data/" + filename);
void FileManager::manageGet(const std::string &filename)
{
	Logger &logger = Logger::getInstance();
	std::ifstream file("data/" + filename);
	std::string line;

	logger.log(DEBUG, "manageGet method is called");
	logger.log(DEBUG, "filename: data/" + filename);

	if (fs::exists("data/" + filename))
	{
		if (!file.is_open())
		{
			logger.log(WARNING, "401 UNAUTHORIZED");
			_status_code = 401;
			return;
		}
		while (std::getline(file, line))
			_content += line + "\n";
		logger.log(INFO, "200 OK");
		_status_code = 200;
	}
	else
	{
		logger.log(WARNING, "404 NOT FOUND");
		_status_code = 404;
	}
	logger.log(DEBUG, "_status_code in the manageGet method is: %",
			   _status_code);
}

// again, here below hard code, handle with input config file?
// 	std::ofstream	newFile("data/upload/" + filename);
void FileManager::managePost(const std::string &filename,
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
void FileManager::manageDelete(const std::string &filename)
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

void FileManager::manage(HTTPMethod method, const std::string &filename,
						 const std::string &body)
{
	Logger &logger = Logger::getInstance();
	logger.log(DEBUG, "manage method is called");
	if (method == HTTPMethod::GET)
		manageGet(filename);
	else if (method == HTTPMethod::POST)
		managePost(filename, body);
	else
		manageDelete(filename);
	logger.log(DEBUG, "_status_code in the manage method is: %", _status_code);
}

const std::string &FileManager::getContent() const
{
	return (_content);
}

int FileManager::getStatusCode() const
{
	return (_status_code);
}
