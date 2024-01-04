#include <ClientException.hpp>
#include <FileManager.hpp>
#include <Logger.hpp>

#include <filesystem>
#include <string>

FileManager::FileManager() : _response(), _request_target()
{
}

FileManager::~FileManager()
{
}

void FileManager::openGetFile(const std::string &request_target_path)
{
	if (!std::filesystem::exists(request_target_path.substr(1)))
		throw ClientException(StatusCode::NotFound);
	_request_target.open(request_target_path.substr(1),
						 std::ios::in | std::ios::binary);
	if (!_request_target.is_open())
		throw ClientException(StatusCode::NotFound);
	HTTPStatus status(StatusCode::OK);
	_response += status.getStatusLine("HTTP/1.1");
}

void FileManager::openPostFile(const std::string &request_target_path)
{
	if (!std::filesystem::exists(request_target_path))
	{
		_request_target.open(request_target_path, std::ios::out);
		if (!_request_target.is_open())
			throw ClientException(StatusCode::InternalServerError);
		HTTPStatus status(StatusCode::Created);
		_response += status.getStatusLine("HTTP/1.1");
	}
	else
	{
		_request_target.open(request_target_path,
							 std::ios::out | std::ios::app);
		if (!_request_target.is_open())
			throw ClientException(StatusCode::InternalServerError);
		HTTPStatus status(StatusCode::OK);
		_response += status.getStatusLine("HTTP/1.1");
	}
}

ClientState FileManager::openErrorPage(const std::string &error_pages_path,
									   const StatusCode &status_code)
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "openErrorPage method is called");
	_request_target.open(error_pages_path.substr(1) +
						 std::to_string(static_cast<int>(status_code)) +
						 ".html");
	if (!_request_target.is_open())
	{
		HTTPStatus status(status_code);
		_response += status.getHTMLStatus();
		return (ClientState::Sending);
	}
	return (ClientState::Error);
}

// still need some elaboration for this part. it seems odd to append the buffer
// to the respons even after it goes into the if(_request_target.bad()). and if
// you don't go in it you've missed your statusline for the response.
ClientState FileManager::loadErrorPage(void)
{
	char buffer[BUFFER_SIZE];
	_request_target.read(buffer, BUFFER_SIZE);
	if (_request_target.bad())
	{
		HTTPStatus status(StatusCode::InternalServerError);
		_response = status.getStatusLine("HTTP/1.1") + status.getHTMLStatus();
	}
	_response += std::string(buffer);
	if (_request_target.eof())
		return (ClientState::Sending);
	return (ClientState::Loading);
}

ClientState FileManager::manageGet(void)
{
	Logger &logger = Logger::getInstance();
	char buffer[BUFFER_SIZE + 1];

	logger.log(DEBUG, "manageGet method is called:");
	_request_target.read(buffer, BUFFER_SIZE);
	if (_request_target.bad())
		throw ClientException(StatusCode::InternalServerError);
	buffer[_request_target.gcount()] = '\0';
	_response += std::string(buffer, _request_target.gcount());
	if (_request_target.eof())
		return (ClientState::Sending);
	return (ClientState::Loading);
}

ClientState FileManager::managePost(const std::string &body)
{
	Logger &logger = Logger::getInstance();
	size_t pos = _request_target.tellp();

	logger.log(DEBUG, "managePost method is called");
	_request_target.write(body.c_str() + pos, BUFFER_SIZE);
	logger.log(DEBUG, "post buffer: " + body.substr(pos, BUFFER_SIZE));
	if (_request_target.fail())
		throw ClientException(StatusCode::InternalServerError);
	if (_request_target.eof())
		return (ClientState::Sending);
	return (ClientState::Loading);
}

ClientState FileManager::manageDelete(const std::string &request_target_path)
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "manageDelete method is called");
	if (std::remove(request_target_path.c_str()) != 0)
		throw ClientException(StatusCode::NotFound);
	HTTPStatus status(StatusCode::NoContent);
	_response += status.getStatusLine("HTTP/1.1");
	return (ClientState::Sending);
}

ClientState FileManager::manage(HTTPMethod method,
								const std::string &request_target_path,
								const std::string &body)
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "FileManager::manage: " +
						  std::to_string(static_cast<int>(method)) + " " +
						  request_target_path + " " + body);
	if (method == HTTPMethod::DELETE)
		return (manageDelete(request_target_path));
	if (method == HTTPMethod::GET)
	{
		if (!_request_target.is_open())
			openGetFile(request_target_path);
		return (manageGet());
	}
	else if (method == HTTPMethod::POST)
	{
		if (!_request_target.is_open())
			openPostFile(request_target_path);
		return (managePost(body));
	}
	return (ClientState::Unkown);
}

const std::string &FileManager::getResponse(void) const
{
	return (_response);
}
