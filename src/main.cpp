#include <WebServ.hpp>
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc != 1 && argc != 2)
		return (EXIT_FAILURE);
	WebServ webserver(argc == 2 ? argv[1] : DEFAULT_CONFIG_PATH);
	return (webserver.run());
}
