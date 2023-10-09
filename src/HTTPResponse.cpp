// #include "../include/HTTPResponse.hpp"
// #include <sstream>
// #include <string>
// #include <Logger.hpp>

// HTTPResponse::HTTPResponse(std::string version, int status_code, std::string body):
// 	_version(version), _status_code(status_code), _body(body) {
// }

// HTTPResponse::~HTTPResponse() {
// }

// void	HTTPResponse::setVersion(const std::string& requestLine) {
// 	std::istringstream	stream_line(requestLine);
// 	std::string word;
// 	stream_line >> word >> word >> word;
// 	_version = word;
// }

// const std::string& HTTPResponse::getVersion(void) const {
// 	return (_version);
// }

// void	HTTPResponse::setHeader(const std::string& key, const std::string& value) {

// 	_headers[key] = value;
// }

// std::string&	HTTPResponse::getValue(const std::string& key) {
// 	return (_headers[key]);
// }

// void	HTTPResponse::setBody(const std::string& body) {
// 	_body = body;
// }

// const std::string&	HTTPResponse::getBody(void) const {
// 	return (_body);
// }

// std::string	HTTPResponse::getResponsePhrase(void) const {
// 	if (_status_code == 200)
// 		return ("OK");
// 	else if (_status_code == 201)
// 		return ("CREATED");
// 	else if (_status_code == 204)
// 		return ("NO CONTENT");
// 	else if (_status_code == 304)
// 		return ("NOT MODIFIED");
// 	else if (_status_code == 400)
// 		return ("BAD REQUEST");
// 	else if (_status_code == 401)
// 		return ("UNAUTHORIZED");
// 	else if (_status_code == 403)
// 		return ("FORBIDDEN");
// 	else
// 		return ("NOT FOUND");	
	
// }

// void	HTTPResponse::createHTTPResponse() {
// 	Logger  		&logger = Logger::getInstance();
	
// 	_http_response_str += _version + ' ';
// 	_http_response_str += std::to_string(_status_code) + ' ';
// 	_http_response_str += getResponsePhrase() + "\r\n";
// 	_http_response_str += "Content-type: " + getValue("Content-type") + "\r\n";
// 	_http_response_str += "Content-length: " + getValue("Content-length") + "\r\n\r\n";
// 	_http_response_str += _body;
// 	logger.log(DEBUG, "HTTP Response: " + _http_response_str);
// }

// std::string	HTTPResponse::getHTTPResponse(void) const {
// 	return (_http_response_str);
// }

// int	HTTPResponse::getStatusCode(void) const {
// 	return (_status_code);
// }