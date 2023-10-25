#include <ClientException.hpp>
#include <FileManager.hpp>
#include <Logger.hpp>

#include <filesystem>

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
	_request_target.open(
		request_target_path); // TODO: Status when file is created
	if (!_request_target.is_open())
		throw ClientException(StatusCode::Forbidden);
}

ClientState FileManager::openErrorPage(const std::string &error_pages_path,
									   const StatusCode &status_code)
{
	_request_target.open(error_pages_path +
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

ClientState FileManager::loadErrorPage(void)
{
	char buffer[BUFFER_SIZE];
	_request_target.read(buffer, BUFFER_SIZE);
	if (_request_target.fail())
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
	char buffer[BUFFER_SIZE];

	logger.log(DEBUG, "manageGet method is called");
	_request_target.read(buffer, BUFFER_SIZE);
	if (_request_target.fail())
		throw ClientException(StatusCode::InternalServerError);
	_response += std::string(buffer);
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
	_response = status.getStatusLine("HTTP/1.1");
	return (ClientState::Sending);
}

ClientState FileManager::manage(HTTPMethod method,
								const std::string &request_target_path,
								const std::string &body)
{
	if (method == HTTPMethod::DELETE)
		return (manageDelete(request_target_path));
	if (!_request_target.is_open())
		openFile(request_target_path);
	if (method == HTTPMethod::GET)
		return (manageGet());
	return (managePost(body));
}

const std::string &FileManager::getResponse(void) const
{
	return (_response);
}
