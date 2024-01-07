#ifndef AUTOINDEXGENERATOR_HPP
#define AUTOINDEXGENERATOR_HPP

#include "Logger.hpp"

#include <dirent.h>
#include <fstream>
#include <string>

namespace AutoIndexGenerator
{
std::fstream OpenAutoIndex(std::string directory, const std::string uri);
std::string AutoIndexGenerator(const std::string dir, const std::string uri);

} // namespace AutoIndexGenerator

#endif // !AUTOINDEXGENERATOR_HPP
