#include "FileManager.hpp"
#include "AutoIndexGenerator.hpp"
#include "ClientException.hpp"
#include "Logger.hpp"
#include "ReturnException.hpp"
#include "StatusCode.hpp"

#include <filesystem>
#include <string>

FileManager::FileManager()
	: _response(), _request_target(), _serversetting(), _autoindex(false),
	  _bytes_sent(0)
{
}

FileManager::~FileManager()
{
}

std::string FileManager::resolveRequestTarget(const std::string &request_target)
{
	Logger &logger = Logger::getInstance();

	const LocationSettings &loc =
		_serversetting.resolveLocation(request_target);
	const std::string root = _serversetting.getRoot().substr(1);

	logger.log(DEBUG, "resolveRequestTarget:\t" + root + " " + request_target);
	if (!loc.getRedirect().empty())
		throw ReturnException(StatusCode::Found, loc);

	if (request_target.back() != '/')
		return (root + loc.resolveAlias(request_target));
	// is a directory
	if (loc.getIndex().empty() == false)
		return (root + loc.resolveAlias(request_target) + loc.getIndex());
	logger.log(DEBUG, "resolveRequestTarget:\tAutoIndex:\t" +
						  (loc.getAutoIndex() ? std::string(" ON")
											  : std::string(" OFF")));
	if (loc.getAutoIndex() == false)
		throw ClientException(StatusCode::UnAuthorized);
	const std::string AI_target = root + loc.resolveAlias(request_target);
	_request_target =
		AutoIndexGenerator::OpenAutoIndex(AI_target, request_target);
	_autoindex = true;
	return (AI_target);
}

void FileManager::openGetFile(const std::string &request_target_path)
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "request_target:\t" + request_target_path);
	const std::string resolved_target =
		resolveRequestTarget(request_target_path);
	logger.log(DEBUG, "resolved_target:\t" + resolved_target);

	if (_autoindex == true)
	{
		HTTPStatus status(StatusCode::OK);
		_response += status.getStatusLine("HTTP/1.1");
		return;
	}

	if (!std::filesystem::exists(resolved_target))
		throw ClientException(StatusCode::NotFound);
	_request_target.open(resolved_target, std::ios::in | std::ios::binary);
	if (!_request_target.is_open())
		throw ClientException(StatusCode::NotFound);
	HTTPStatus status(StatusCode::OK);
	_response += status.getStatusLine("HTTP/1.1");
}

void FileManager::openPostFile(const std::string &request_target_path)
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "request_target:\t" + request_target_path);
	const std::string resolved_target =
		resolveRequestTarget(request_target_path);
	logger.log(DEBUG, "resolved_target:\t" + resolved_target);

	if (!std::filesystem::exists(resolved_target))
	{
		_request_target.open(resolved_target, std::ios::out);
		if (!_request_target.is_open())
			throw ClientException(StatusCode::InternalServerError);
		HTTPStatus status(StatusCode::Created);
		_response += status.getStatusLine("HTTP/1.1");
	}
	else
	{
		_request_target.open(resolved_target, std::ios::out | std::ios::trunc);
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

	logger.log(DEBUG, "openErrorPage method is called. (" + error_pages_path +
						  std::to_string(static_cast<int>(status_code)) +
						  ".html)");
	_request_target.open(error_pages_path +
							 std::to_string(static_cast<int>(status_code)) +
							 ".html",
						 std::ios::in);
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
	buffer[_request_target.gcount()] = '\0';
	_response += std::string(buffer, _request_target.gcount());
	if (_request_target.eof())
	{
		return (ClientState::Sending);
	}
	return (ClientState::Error);
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

	logger.log(DEBUG, "request_target:\t" + request_target_path);
	const std::string resolved_target =
		resolveRequestTarget(request_target_path);
	logger.log(DEBUG, "resolved_target:\t" + resolved_target);

	logger.log(DEBUG, "manageDelete method is called");
	if (std::remove(resolved_target.c_str()) != 0)
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

	logger.log(DEBUG, "FileManager::manage:");

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
	return (ClientState::Unknown);
}

ClientState FileManager::manageCgi(std::string http_version,
								   const std::string &body)
{
	_response = http_version + " 200 OK\t\n\t\n" + body;
	return (ClientState::Sending);
}

const std::string &FileManager::getResponse(void) const
{
	return (_response);
}

void FileManager::addToResponse(const std::string str)
{
	_response += str;
}

void FileManager::setResponse(const std::string str)
{
	_response = str;
}

void FileManager::setServerSetting(const ServerSettings &serversetting)
{
	_serversetting = serversetting;
}
