#include "StatusCode.hpp"
#include <HTTPStatus.hpp>

HTTPStatus::HTTPStatus(StatusCode status_code) : _status_code(status_code)
{
	_message[StatusCode::OK] = "OK";
	_message[StatusCode::Created] = "Created";
	_message[StatusCode::Accepted] = "Accepted";
	_message[StatusCode::NoContent] = "No Content";
	_message[StatusCode::Found] = "Found";
	_message[StatusCode::NotModified] = "Not Modified";
	_message[StatusCode::BadRequest] = "Bad Request";
	_message[StatusCode::UnAuthorized] = "Unauthorized";
	_message[StatusCode::Forbidden] = "Forbidden";
	_message[StatusCode::NotFound] = "Not Found";
	_message[StatusCode::MethodNotAllowed] = "Method Not Allowed";
	_message[StatusCode::RequestTimeout] = "Request Timeout";
	_message[StatusCode::LenghtRequired] = "Length Required";
	_message[StatusCode::PayloadToLarge] = "Payload Too Large";
	_message[StatusCode::URIToLong] = "URI Too Long";
	_message[StatusCode::UnsupportedMediaType] = "Unsupported Media Type";
	_message[StatusCode::InternalServerError] = "Internal Server Error";
	_message[StatusCode::NotImplemented] = "Not Implemented";
	_message[StatusCode::BadGateway] = "Bad Gateway";
	_message[StatusCode::ServiceUnavailable] = "Service Unavailable";
	_message[StatusCode::GatewayTimeout] = "Gateway Timeout";
}

HTTPStatus::~HTTPStatus()
{
}

std::string HTTPStatus::getStatusLine(const std::string &version) const
{
	return (version + " " + std::to_string(static_cast<int>(_status_code)) +
			" " + _message.at(_status_code));
}

StatusCode HTTPStatus::getStatusCode() const
{
	return (_status_code);
}
