#include <Webserver.hpp>

Webserver::Webserver()
{
}

Webserver::Webserver(const std::string &config_file) : _config_file(config_file)
{
}

Webserver::~Webserver()
{
}
