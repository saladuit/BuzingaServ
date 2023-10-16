#ifndef SYSTEM_EXCEPTION_HPP
#define SYSTEM_EXCEPTION_HPP

#include <cstring>
#include <stdexcept>
#include <string>

class SystemException : public std::runtime_error
{
  public:
	SystemException(const std::string &function_name)
		: std::runtime_error("System exception in function: " + function_name +
							 strerror(errno)){};
};

#endif
