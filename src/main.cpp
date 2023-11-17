#include <ConfigParser.hpp>
#include <HTTPServer.hpp>
#include <Logger.hpp>
#include <stdexcept>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
	Logger &logger = Logger::getInstance();
	const std::string program_name(argv[0]);

	logger.log(LogLevel::INFO, "Launching server");
	if (argc != 1 && argc != 2)
	{
		logger.log(LogLevel::ERROR, "Usage: " + program_name + "[config_path]");
		return (EXIT_FAILURE);
	}
	HTTPServer server(argc == 2 ? argv[1] : "config/default.conf");
	return (server.run());
}

// &&&&&&&&&&&&&&

// #include <fcntl.h>
// #include <unistd.h>
// int	main(void) {

// 	Logger		&logger = Logger::getInstance();
// 	CGI			cgi_request;
// 	std::string	body;

// 	cgi_request.execute("data/www/python/test.py", true, body);
// 	logger.log(INFO, "Body: " + body);

// 	return 0;

	// &

	// std::string bin = "python3";
	// std::string script = "data/www/python/test.py";
	// const char *path = "/usr/bin/python3";

	// const char *const argv[] = {bin.c_str(), script.c_str(), NULL};
	// char *const envp[] = {NULL};

	// execve(path, (char *const *)argv, envp);
	// return 0;
// }

// ######

// //#define REQUEST "GET /index HTTP/1.1 balllen \r\nHost:
// 192.168.0.199:80\r\nConnection: keep-alive\r\nAccept: */*\r\nUser-Agent:
// Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.22 (KHTML, like Gecko)
// Chrome/25.0.1364.97 Safari/537.22\r\nAccept-Encoding:
// gzip,deflate,sdch\r\nAccept-Language: en-US,en;q=0.8\r\nAccept-Charset:
// ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\nballen" #define REQUEST "GET /index
// HTTP/1.1\r\nHost: 192.168.0.199:80\r\nConnection: keep-alive\r\nAccept:
// */*\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.22
// (KHTML, like Gecko)     Chrome/25.0.1364.97 Safari/537.22\r\nAccept-Encoding:
// gzip,deflate,sdch\r\nAccept-Language: en-US,en;q=0.8\r\nAccept-Charset:
// ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\n\r\nthis is the message body\r\njoe"

// int	main(void) {
// 	// LOGGER AND CLASS VARIABLE
// 	Logger		&logger = Logger::getInstance();
// 	HTTPRequest	test;

// 	// PARSING REQUEST
// 	test.parse(REQUEST);

// 	// LOG INFO
// 	std::string header = "Host";
// 	logger.log(LogLevel::INFO, "Method: %", (int) test.getMethodType());
// 	logger.log(LogLevel::INFO, "Path: %", test.getPath());
// 	logger.log(LogLevel::INFO, "Version: %", test.getVersion());
// 	logger.log(LogLevel::INFO, "Value (with \"%\" as key): %", header,
// test.getValue(header)); 	logger.log(LogLevel::INFO, "Body: %",
// test.getBody()); 	return 0;
