#include <ConfigParser.hpp>
#include <Logger.hpp>
// #include <HTTPServer.hpp>
// #include <cstdlib>

#define PATH "data/www/index.html"

int main() {
	try {
		std::string filename = PATH;
		std::string fileContent = readFileToString(filename);
		std::cout << "File content:\n" << fileContent;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

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
