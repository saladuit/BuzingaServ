#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

// INCLUDES
#include <string>

// ENUM
enum class HTTPMethod {
	GET,
	POST,
	DELETE,
};

// CLASS
class FileManager {
  private:
	int			_statusCode;
	std::string	_content;

  public:
	// canonical form: constructors, operator and destructor
	FileManager();
	FileManager(const FileManager& other) = delete;
	void	operator=(const FileManager& other) = delete;
	~FileManager();

	// manage methods
	void	manage(HTTPMethod method, const std::string& filename, const std::string& body);
	void	manageGet(const std::string& filename);
	void	managePost(const std::string& filename, const std::string& body);
	void	manageDelete(const std::string& filename);

	// getters
	std::string getContent();
	int 		getStatusCode();

};

#endif