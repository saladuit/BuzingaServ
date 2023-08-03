#ifndef WebServ_HPP
#define WebServ_HPP

#include <Color.hpp>
#include <Defines.hpp>
#include <cstdlib>
#include <iostream>

class WebServ
{
  public:
	WebServ(const std::string &config_path);
	~WebServ();
	int run();

  private:
	const std::string _config_path;

	WebServ();
	WebServ(const WebServ &src);
	WebServ &operator=(const WebServ &rhs);
};
#endif

/* ************************************************************************** */
