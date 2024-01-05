#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include "HTTPRequest.hpp"
#include "HTTPStatus.hpp"
#include "LocationSettings.hpp"
#include "ServerSettings.hpp"

#include <fstream>
#include <string>

class FileManager
{
  private:
	std::string _response;
	std::fstream _request_target;
	const ServerSettings &_serversetting;
	bool _autoindex;

	std::string applyLocationSettings(const std::string &request_target,
									  HTTPMethod method);

  public:
	FileManager();
	FileManager(const ServerSettings &ServerSettings);
	FileManager(const FileManager &other) = delete;
	void operator=(const FileManager &other) = delete;
	~FileManager();

	void openGetFile(const std::string &request_target_path);
	void openPostFile(const std::string &request_target_path);
	ClientState openErrorPage(const std::string &error_pages_path,
							  const StatusCode &status_code);
	ClientState setErrorResponse(const StatusCode &status_code);
	ClientState loadErrorPage(void);
	ClientState manage(HTTPMethod method, const std::string &filename,
					   const std::string &body);
	ClientState manageGet(void);
	ClientState managePost(const std::string &body);
	ClientState manageDelete(const std::string &reqest_target_path);

	const std::string &getResponse(void) const;
};

#endif
