#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

// INCLUDES
#include <string>

class FileManager {
  private:
	int			_statusCode;
	std::string	_content;

  public:
	FileManager();
	FileManager(const FileManager& other) = delete;
	void	operator=(const FileManager& other) = delete;
	~FileManager();

	void manage(const std::string& filename);
	std::string getContent(void);
	int 		getStatusCode(void);

};

#endif