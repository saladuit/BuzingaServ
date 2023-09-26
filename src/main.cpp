#include <HTTPServer.hpp>
#include <Logger.hpp>

int main(int argc, char **argv)
{
	Logger &logger = Logger::getInstance();
	if (argc != 1 && argc != 2)
	{
		logger.log(LogLevel::ERROR, "Usage: % [config_path]", argv[0]);
		return (EXIT_FAILURE);
	}
	HTTPServer server(argc == 2 ? argv[1] : "config/default.conf");
	return (server.run());
}
