#include <ConfigParser.hpp>
#include <Logger.hpp>
// #include <HTTPServer.hpp>
// #include <cstdlib>
#include "../include/FileManager.hpp"

#define PATH "data/www/index.htmlg"
std::string manage(const std::string& filename);

int main() {
	Logger	&logger = Logger::getInstance();
	FileManager	file;

	file.manage(PATH);
	if (file.getContent().empty()) {
		logger.log(ERROR, "Status_code: %, NOT FOUND", file.getStatusCode());
		return (0);
	}

	logger.log(INFO, "Status_code: %, Body:\n\n%", file.getStatusCode(), file.getContent());
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
