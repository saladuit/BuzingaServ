#include "ConfigParser.hpp"
#include "Logger.hpp"
#include "../include/HTTPRequest.hpp"
// #include <HTTPServer.hpp>
// #include <cstdlib>

//#define REQUEST "GET /index HTTP/1.1 balllen \r\nHost: 192.168.0.199:80\r\nConnection: keep-alive\r\nAccept: */*\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.22 (KHTML, like Gecko)     Chrome/25.0.1364.97 Safari/537.22\r\nAccept-Encoding: gzip,deflate,sdch\r\nAccept-Language: en-US,en;q=0.8\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\nballen"
#define REQUEST "GET /index HTTP/1.1\r\nHost: 192.168.0.199:80\r\nConnection: keep-alive\r\nAccept: */*\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.22 (KHTML, like Gecko)     Chrome/25.0.1364.97 Safari/537.22\r\nAccept-Encoding: gzip,deflate,sdch\r\nAccept-Language: en-US,en;q=0.8\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\n\r\nthis is the message body\r\njoe"

int	main(void) {
	// LOGGER AND CLASS VARIABLE
	Logger		&logger = Logger::getInstance();
	HTTPRequest	test;

	// PARSING REQUEST
	test.parse(REQUEST);

	// LOG INFO
	std::string header = "Host";
	logger.log(LogLevel::INFO, "Method: %", (int) test.getMethodType());
	logger.log(LogLevel::INFO, "Path: %", test.getPath());
	logger.log(LogLevel::INFO, "Version: %", test.getVersion());
	logger.log(LogLevel::INFO, "Value (with \"%\" as key): %", header, test.getValue(header));
	logger.log(LogLevel::INFO, "Body: %", test.getBody());
	return 0;

}

//int main(int argc, char **argv)
//{
//	Logger &logger = Logger::getInstance();
//	if (argc != 1 && argc != 2)
//	{
//		logger.log(LogLevel::ERROR, "Usage: % [config_path]", argv[0]);
//		return (EXIT_FAILURE);
//	}
//	ConfigParser parser(argc == 2 ? argv[1] : "config/default.conf");
//
//	parser.readConfig();
//}
// int main(int argc, char **argv)
// {
// 	return (EXIT_FAILURE);
// 	try
// 	{
// 		HTTPServer server(argc == 2 ? argv[1] : DEFAULT_CONFIG_PATH);
// 		return (server.run());
// 	}
// 	catch (const std::exception &e)
// 	{
// 		Logger::log(LOG_ERROR, e.what());
// 	}
// 	return (EXIT_FAILURE);
// }
