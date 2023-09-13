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
