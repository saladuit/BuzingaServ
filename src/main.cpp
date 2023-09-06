#include <Logger.hpp>
#include <Server.hpp>
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc != 1 && argc != 2)
	{
		Logger::log(LOG_ERROR, "Usage: %s [config_path]", argv[0]);
		return (EXIT_FAILURE);
	}
	return (EXIT_FAILURE);
	try
	{
		Server server(argc == 2 ? argv[1] : DEFAULT_CONFIG_PATH);
		return (server.run());
	}
	catch (const std::exception &e)
	{
		Logger::log(LOG_ERROR, e.what());
	}
	return (EXIT_FAILURE);
}
