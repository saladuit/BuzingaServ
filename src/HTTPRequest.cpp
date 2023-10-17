#include "../include/HTTPRequest.hpp"
#include <sstream>
#include <string>
#include <Logger.hpp>

HTTPRequest::HTTPRequest(): _methodType(HTTPMethod::GET), _content_length(0), 
		_content_length_cpy(0), _post_method(false), _pos(0) {
}

HTTPRequest::HTTPRequest(const HTTPRequest& other) {
	*this = other;	
}

HTTPRequest&	HTTPRequest::operator=(const HTTPRequest& other) {
	if (this != &other)
	{
		this->_content_length = other._content_length;
		this->_content_length_cpy = other._content_length_cpy;
		this->_post_method = other._post_method;
		this->_pos = other._pos;
	}
	return (*this);
}

HTTPRequest::~HTTPRequest() {
}

void	HTTPRequest::setMethodType(const std::string& requestLine) {
	std::istringstream	stream_line(requestLine);
	std::string word;
	stream_line >> word;
	if (word == "GET")
		_methodType = HTTPMethod::GET;
	else if (word == "POST")
		_methodType = HTTPMethod::POST;
	else if (word == "DELETE")
		_methodType = HTTPMethod::DELETE;
	else
		_methodType = HTTPMethod::UNKNOWN;
}

HTTPMethod HTTPRequest::getMethodType(void) const {
	return (_methodType);
}

void	HTTPRequest::setPath(const std::string& requestLine) {
	std::istringstream	stream_line(requestLine);
	std::string word;
	stream_line >> word >> word;
	_path = word;
}

const std::string& HTTPRequest::getPath(void) const {
	return (_path);
}

void	HTTPRequest::setVersion(const std::string& requestLine) {
	std::istringstream	stream_line(requestLine);
	std::string word;
	stream_line >> word >> word >> word;
	_version = word;
}

const std::string& HTTPRequest::getVersion(void) const {
	return (_version);
}

void	HTTPRequest::setHeader(const std::string& headerLine) {
	size_t		startPos = 0;
	size_t		foundPos = headerLine.find(": ", startPos);
	std::string	key;
	std::string	value;
	size_t		length;

	if (foundPos != std::string::npos) {
		length = foundPos - startPos;
		std::string substring = headerLine.substr(startPos, length);
		key = substring;
	}
	startPos = foundPos + 2;
	length = headerLine.length() - startPos;
	value = headerLine.substr(startPos, length);

	_headers[key] = value;
}

std::string&	HTTPRequest::getValue(const std::string& key) {
	return (_headers[key]);
}

void	HTTPRequest::setBody(const std::string& body) {
	_body = body;
}

const std::string&	HTTPRequest::getBody(void) const {
	return (_body);
}

void HTTPRequest::parse(void) {
	Logger 		&logger = Logger::getInstance();
	bool		bodyLine = false;
	bool		firstLine = true;
	size_t		startPos = 0;

	logger.log(DEBUG, "_http_request_str: %", _http_request_str);
	logger.log(INFO, "HTTPRequest::parse is called");
	while (startPos != std::string::npos)
	{
		size_t	foundPos = _http_request_str.find("\r\n", startPos);
		if (foundPos != std::string::npos)
		{
			size_t length = foundPos - startPos;
			std::string substring = _http_request_str.substr(startPos, length);
			logger.log(DEBUG, "substring: %", substring);
			if (substring == "")
			{
				bodyLine = true;
				startPos = foundPos + 2;
				continue ;
			}
			if (firstLine)
			{
				setMethodType(substring);
				setPath(substring);
				setVersion(substring);
				firstLine = false;
			}
			else if (bodyLine)
			{
				substring = _http_request_str.substr(startPos, _content_length_cpy + 1);
				setBody(substring);
				break ;
			}
			else
			{
				setHeader(substring);
			}
			startPos = foundPos + 2;
		}
		else
			break ;
	}
}
