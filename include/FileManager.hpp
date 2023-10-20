#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <HTTPRequest.hpp>
#include <string>

class FileManager
{
  private:
	int _status_code;
	std::string _content;

  public:
	// canonical form: constructors, operator and destructor
	FileManager();
	FileManager(const FileManager &other) = delete;
	void operator=(const FileManager &other) = delete;
	~FileManager();

	// manage methods
	void openFile(std::string &request_target);
	void manage(HTTPMethod method, const std::string &filename,
				const std::string &body);
	void manageGet(const std::string &filename);
	void managePost(const std::string &filename, const std::string &body);
	void manageDelete(const std::string &filename);

	// getters
	const std::string &getContent() const;
	int getStatusCode() const;
};

#endif
