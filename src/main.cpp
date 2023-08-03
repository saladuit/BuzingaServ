#include <Webserver.hpp>
#include <cstdlib>

int main(int argc, char **argv)
{
	if (argc != 2 && argc != 3)
		return (EXIT_FAILURE);
	if (*argv[1] == '\0')
		return (EXIT_FAILURE);
	return (webserver.run());
}
