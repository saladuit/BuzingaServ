#include <ClientException.hpp>
#include <StatusCode.hpp>
#include <HTTPRequest.hpp>
#include <Logger.hpp>
#include <SystemException.hpp>

#include <climits>
#include <string>

#include <unistd.h>

HTTPRequest::HTTPRequest(const ServerSettings &serversetting)
	: _bytes_read(0), _content_length(0), _max_body_size(), 
	  _methodType(HTTPMethod::UNKNOWN), _http_request(), _request_target(),
	  _http_version(), _body(), _serversetting(serversetting), _headers()
{
	setMaxBodySize(_serversetting.getClientMaxBodySize());
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
		_methodType = HTTPMethod::UNKNOWN; // TODO: throw exception
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

ssize_t HTTPRequest::getMaxBodySize(void) const
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

size_t HTTPRequest::parseStartLine(size_t &i)
{
	Logger &logger = Logger::getInstance();
	size_t pos;

	pos = _http_request.find(' ', i);
	setMethodType(_http_request.substr(i, pos - i));
	i = pos + 1;
	pos = _http_request.find(' ', i);

	std::string prelim = _http_request.substr(i, pos - i);
	const LocationSettings &loc = _serversetting.resolveLocation(prelim);
	if (prelim.find_last_of('/') == prelim.length() - 1)
		logger.log(WARNING, "prelim end with /\t" + prelim);

	setRequestTarget(loc.resolveAlias(prelim));
	if (loc.resolveMethod(_methodType) == false)
		throw ClientException(StatusCode::Forbidden);
	
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

ClientState HTTPRequest::receive(int client_fd)
{
	Logger &logger = Logger::getInstance();
	char buffer[BUFFER_SIZE];
	std::string header_end;
	size_t pos;
	size_t i = 0;

	_bytes_read = read(client_fd, buffer, BUFFER_SIZE);
	if (_bytes_read == SYSTEM_ERROR)
		throw SystemException("Read failed on: " + std::to_string(client_fd));
	if (_content_length != 0)
	{
		_body += std::string(buffer, _bytes_read);
		if (_body.size() >= _content_length)
		{
			logger.log(DEBUG, "Body: " + _body);
			return (ClientState::Loading);
		}
		if (_body.size() >= _max_body_size) // @saladuit not sure if this should be before the previous ifstatement
			throw ClientException(StatusCode::RequestBodyTooLarge);
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
		{
			if (_headers.find("Content-length") != _headers.end())
			{
				_content_length = std::stoi(getHeader("Content-length"));
				if (_content_length == 0)
					return (ClientState::Loading);
				_body += _http_request.substr(pos + 2);
				return (ClientState::Receiving);
			}
			else
				return (ClientState::Loading);
		}
		pos = parseHeaders(i);
	}
	return (ClientState::Receiving);
}
