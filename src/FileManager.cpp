#include <iostream>
#include <fstream>
#include <string>

std::string readFileToString(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		// Handle the case where the file could not be opened
		throw std::runtime_error("Failed to open file: " + filename);
	}

	std::string content;
	std::string line;

	while (std::getline(file, line)) {
		content += line + "\n"; // Add newline character if needed
	}

	return content;
}
