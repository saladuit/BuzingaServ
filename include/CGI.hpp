#ifndef CGI_HPP
#define CGI_HPP

#define READ_END 0
#define WRITE_END 1

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
