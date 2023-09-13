#include "http_request.hpp"
#include <string>
#include <sstream>

HTTPRequest::HTTPRequest() {
	_methodType = 0;
}

HTTPRequest::~HTTPRequest() {
}

void	HTTPRequest::setMethodType(const std::string& requestLine) {
	std::istringstream	stream_line(requestLine);
	std::string word;
	stream_line >> word;
	if (word == _GET)
		_methodType = (int) HTTPMethod::GET;
	else if (word == _POST)
		_methodType = (int) HTTPMethod::POST;
	else if (word == _DELETE)
		_methodType = (int) HTTPMethod::DELETE;
	else
		_methodType = (int) HTTPMethod::UNKNOWN;
}

int HTTPRequest::getMethodType(void) const {
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
