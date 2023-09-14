#include <iostream>
#include <filesystem>
#include <string>

#define PATH "data/www/index.html"
//std::string	FileManager(std::string path) {
int	FileManager() {
	std::string	content;

	const std::filesystem::path projectPath = "data/www";
	const std::string filenameToFind = "index.html";

	for (const auto& entry : std::filesystem::recursive_directory_iterator(projectPath)) {
		if (entry.is_regular_file() && entry.path().filename() == filenameToFind) {
			std::cout << "File found: " << entry.path() << std::endl;
			return 0; // File found, exit with success status
		}
	}
	std::cout << "File not found." << std::endl;
	return 1; // File not found, exit with error status
//	return (content);
}