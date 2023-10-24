#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <HTTPRequest.hpp>
#include <fstream>
#include <string>

class FileManager
{
  private:
	std::string _response;
	std::fstream _request_target;

  public:
	FileManager();
	FileManager(const FileManager &other) = delete;
	void operator=(const FileManager &other) = delete;
	~FileManager();

	void openFile(const std::string &request_target);
	ClientState manage(HTTPMethod method, const std::string &filename,
					   const std::string &body);
	ClientState manageGet(void);
	ClientState managePost(const std::string &body);
	ClientState manageDelete(const std::string &reqest_target_path);

	const std::string &getResponse(void) const;
};

#endif
