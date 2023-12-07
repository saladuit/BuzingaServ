#include <ClientException.hpp>
#include <FileManager.hpp>
#include <Logger.hpp>

#include <algorithm>
#include <filesystem>

FileManager::FileManager() : _response(), _request_target(), _bytes_sent(0)
{
}

FileManager::~FileManager()
{
}

void FileManager::openGetFile(const std::string &request_target_path)
{
	if (!std::filesystem::exists(request_target_path))
		throw ClientException(StatusCode::NotFound);
	_request_target.open(request_target_path, std::ios::in);
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

	logger.log(DEBUG, "manageGet method is called");
	_request_target.read(buffer, BUFFER_SIZE);
	if (_request_target.bad())
		throw ClientException(StatusCode::InternalServerError);
	logger.log(DEBUG, "get buffer: " + std::string(buffer));
	buffer[_request_target.gcount()] = '\0';
	_response += std::string(buffer);
	if (_request_target.eof())
		return (ClientState::Sending);
	return (ClientState::Loading);
}

ClientState FileManager::managePost(const std::string &body)
{
	Logger &logger = Logger::getInstance();

	ssize_t pos = _request_target.tellp();
	size_t bytes_to_send = 0;

	
	logger.log(DEBUG, "managePost method is called");
	logger.log(DEBUG, "pos: %", pos);
	if (body.length() - _bytes_sent < BUFFER_SIZE)
		bytes_to_send = body.length() - _bytes_sent;
	else
		bytes_to_send = BUFFER_SIZE;

	_request_target.write(body.c_str() + pos, bytes_to_send);
	_bytes_sent += bytes_to_send;
	if (_request_target.fail())
		throw ClientException(StatusCode::InternalServerError);
			if (_bytes_sent == body.size())
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

ClientState FileManager::manageCgi(std::string http_version, const std::string &body)
{
	_response = http_version + " 200 OK\t\n\t\n" + body;
	return (ClientState::Sending);
}

const std::string &FileManager::getResponse(void) const
{
	return (_response);
}
