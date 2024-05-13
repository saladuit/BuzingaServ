#include "FileManager.hpp"
#include "AutoIndexGenerator.hpp"
#include "ClientException.hpp"
#include "Logger.hpp"
#include "ReturnException.hpp"
#include "StatusCode.hpp"

#include <filesystem>
#include <string>

FileManager::FileManager()
	: _response(), _request_target(), _serversetting(), _autoindex(false)
{
}

FileManager::~FileManager()
{
}

std::string
FileManager::applyLocationSettings(const std::string &request_target,
								   HTTPMethod method)
{

	const LocationSettings &loc =
		_serversetting.resolveLocation(request_target);

	if (loc.resolveMethod(method) == false)
		throw ClientException(StatusCode::MethodNotAllowed);
	if (!loc.getRedirect().empty())
		throw ReturnException(StatusCode::Found, loc);

	if (request_target.find_last_of('/') == request_target.length() - 1)
	{
		if (!loc.getIndex().empty())
			return (loc.resolveAlias(request_target).substr(1) +
					loc.getIndex());
		else if (loc.getAutoIndex() == false)
			return (loc.resolveAlias(request_target).substr(1) + "index.html");
		_request_target = AutoIndexGenerator::OpenAutoIndex(
			loc.resolveAlias(request_target).substr(1), request_target);
		_autoindex = true;
	}
	return (loc.resolveAlias(request_target).substr(1));
}

void FileManager::openGetFile(const std::string &request_target_path)
{
	const std::string resolved_target =
		applyLocationSettings(request_target_path, HTTPMethod::GET);

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
	const std::string resolved_target =
		applyLocationSettings(request_target_path, HTTPMethod::POST);

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
		_request_target.open(resolved_target, std::ios::out | std::ios::app);
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

	logger.log(DEBUG, "FileManager::manage: ");
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
