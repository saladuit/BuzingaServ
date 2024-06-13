
#include "ClientState.hpp"
#include <ClientException.hpp>
#include <HTTPRequest.hpp>
#include <Logger.hpp>
#include <StatusCode.hpp>
#include <SystemException.hpp>

#include <string>

#include <unistd.h>

HTTPRequest::HTTPRequest()
	: _header_end(false), _bytes_read(0), _content_length(0), _max_body_size(),
	  _methodType(HTTPMethod::UNKNOWN), _http_request(), _request_target(),
	  _http_version(), _body(), _headers(), _cgi(false)
{
}

HTTPRequest::~HTTPRequest()
{
}

void HTTPRequest::setMethodType(const std::string &method_type)
{
	if (method_type == "GET")
		_methodType = HTTPMethod::GET;
	else if (method_type == "POST")
		_methodType = HTTPMethod::POST;
	else if (method_type == "DELETE")
		_methodType = HTTPMethod::DELETE;
	else
		throw ClientException(StatusCode::NotImplemented);
}

HTTPMethod HTTPRequest::getMethodType(void) const
{
	return (_methodType);
}

void HTTPRequest::setMaxBodySize(std::string inp)
{
	size_t pos = inp.find_first_of("KM");
	std::string nbr = inp.substr(0, pos);
	std::string mag;
	if (pos != std::string::npos)
	{
		mag = inp.substr(pos);
		if (mag == "K")
			nbr += "000";
		else // (mag == "M")
			nbr += "000000";
	}

	_max_body_size = std::stoull(nbr);
}

size_t HTTPRequest::getMaxBodySize(void) const
{
	return (_max_body_size);
}

void HTTPRequest::setHeader(const std::string &key, const std::string &header)
{
	_headers.emplace(key, header);
}

const std::string &HTTPRequest::getHeader(const std::string &key) const
{
	return (_headers.at(key));
}

void HTTPRequest::setRequestTarget(const std::string &request_target)
{
	_request_target = request_target;
}

const std::string &HTTPRequest::getRequestTarget(void) const
{
	return (_request_target);
}

void HTTPRequest::setHTTPVersion(const std::string &http_version)
{
	_http_version = http_version;
}

const std::string &HTTPRequest::getHTTPVersion(void) const
{
	return (_http_version);
}

const std::string &HTTPRequest::getBody(void) const
{
	return (_body);
}

const size_t &HTTPRequest::getBodyLength(void) const
{
	return (_content_length);
}

void HTTPRequest::setCGI(bool b)
{
	_cgi = b;
}

const bool &HTTPRequest::getCGI(void) const
{
	return (_cgi);
}

void HTTPRequest::setHeaderEnd(bool b)
{
	_header_end = b;
}

bool HTTPRequest::getHeaderEnd() const
{
	return (_header_end);
}

size_t HTTPRequest::parseStartLine(size_t &i)
{
	Logger &logger = Logger::getInstance();
	size_t pos;

	logger.log(DEBUG, "_http_request: %", _http_request);
	pos = _http_request.find(' ', i);
	setMethodType(_http_request.substr(i, pos - i));

	i = pos + 1;
	pos = _http_request.find(' ', i);
	setRequestTarget(_http_request.substr(i, pos - i));

	i = pos + 1;
	pos = _http_request.find("\r\n", i);
	setHTTPVersion(_http_request.substr(i, pos - i));
	i = pos + 2;
	logger.log(DEBUG, "method: " + std::to_string((int)getMethodType()));
	logger.log(DEBUG, "request_target: " + getRequestTarget());
	logger.log(DEBUG, "http_version: " + getHTTPVersion());
	return (i);
}

size_t HTTPRequest::parseHeaders(size_t &i)
{
	Logger &logger = Logger::getInstance();
	std::string line;
	std::string key;
	std::string value;
	size_t pos;

	pos = _http_request.find("\r\n", i);
	line = _http_request.substr(i, pos - i);
	key = line.substr(0, line.find(':'));
	logger.log(DEBUG, "Key: " + key);
	value = line.substr(line.find(':') + 2);
	logger.log(DEBUG, "Value: " + value);
	setHeader(key, value);
	i = pos + 2;
	return (i);
}

ClientState HTTPRequest::setRequestVariables(size_t pos)
{
	setHeaderEnd(true);
	if (_headers.find("Content-Length") != _headers.end())
		_content_length = std::stoi(getHeader("Content-Length"));

	if (_content_length == 0)
		return (ClientState::Loading);
	_body += _http_request.substr(pos + 2);
	if (_body.size() == _content_length)
		return (ClientState::Loading);
	return (ClientState::Receiving);
}

ClientState HTTPRequest::receive(int client_fd)
{
	Logger &logger = Logger::getInstance();
	char buffer[BUFFER_SIZE];
	std::string header_end;
	size_t i = 0;
	size_t pos;

	_bytes_read = read(client_fd, buffer, BUFFER_SIZE);
	logger.log(WARNING, "receive:" + std::to_string(__LINE__) + " Body: " +
							std::string(buffer, _bytes_read)); // TODO: remove
	if (_bytes_read == SYSTEM_ERROR)
		throw ClientException(StatusCode::InternalServerError);
	logger.log(DEBUG, "in receive _bytes_read is: %", _bytes_read);
	if (_bytes_read == 0)
		return (ClientState::Receiving);

	if (_content_length != 0)
	{
		_body += std::string(buffer, _bytes_read);
		if (_body.size() > _max_body_size)
			throw ClientException(StatusCode::RequestBodyTooLarge);
		if (_body.size() >= _content_length)
			return (ClientState::Loading);
		return (ClientState::Receiving);
	}
	_http_request += std::string(buffer, _bytes_read);
	pos = _http_request.find("\r\n\r\n");
	if (pos != std::string::npos)
		pos = parseStartLine(i);
	while (pos != std::string::npos)
	{
		header_end = _http_request.substr(pos - 2, 4);
		if (header_end == "\r\n\r\n")
			return (setRequestVariables(pos));
		pos = parseHeaders(i);
	}
	return (ClientState::Receiving);
}
