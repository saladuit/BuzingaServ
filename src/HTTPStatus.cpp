#include "StatusCode.hpp"
#include <HTTPStatus.hpp>

std::unordered_map<StatusCode, std::string> HTTPStatus::_message = {
	{StatusCode::OK, "OK"},
	{StatusCode::Created, "Created"},
	{StatusCode::Accepted, "Accepted"},
	{StatusCode::NoContent, "No Content"},
	{StatusCode::Found, "Found"},
	{StatusCode::NotModified, "Not Modified"},
	{StatusCode::BadRequest, "Bad Request"},
	{StatusCode::UnAuthorized, "Unauthorized"},
	{StatusCode::Forbidden, "Forbidden"},
	{StatusCode::NotFound, "Not Found"},
	{StatusCode::MethodNotAllowed, "Method Not Allowed"},
	{StatusCode::RequestTimeout, "Request Timeout"},
	{StatusCode::LenghtRequired, "Length Required"},
	{StatusCode::PayloadToLarge, "Payload Too Large"},
	{StatusCode::URIToLong, "URI Too Long"},
	{StatusCode::UnsupportedMediaType, "Unsupported Media Type"},
	{StatusCode::InternalServerError, "Internal Server Error"},
	{StatusCode::NotImplemented, "Not Implemented"},
	{StatusCode::BadGateway, "Bad Gateway"},
	{StatusCode::ServiceUnavailable, "Service Unavailable"},
	{StatusCode::GatewayTimeout, "Gateway Timeout"}};

HTTPStatus::HTTPStatus(StatusCode status_code) : _status_code(status_code)
{
}

HTTPStatus::~HTTPStatus()
{
}

std::string HTTPStatus::getStatusLine(const std::string &version) const
{
	return (version + " " + std::to_string(static_cast<int>(_status_code)) +
			" " + _message.at(_status_code)) +
		   "\r\n\r\n";
}

std::string HTTPStatus::getHTMLStatus(void) const
{
	return ("<html><body><h1>" +
			std::to_string(static_cast<int>(_status_code)) + " " +
			_message.at(_status_code)) +
		   "</h1></body></html>";
}

StatusCode HTTPStatus::getStatusCode() const
{
	return (_status_code);
}
