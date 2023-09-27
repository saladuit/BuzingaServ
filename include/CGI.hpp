#ifndef CGI_HPP
#define CGI_HPP

// Common gateway interface

class CGI
{
  private:
  public:
	CGI();
	CGI(const CGI &src) = delete;
	CGI &operator=(const CGI &rhs) = delete;
	~CGI();
	void execute();
};

#endif
