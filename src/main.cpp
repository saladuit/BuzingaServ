// #include <HTTPServer.hpp>
// #include <Logger.hpp>

// int main(int argc, char **argv)
// {
// 	Logger &logger = Logger::getInstance();
// 	if (argc != 1 && argc != 2)
// 	{
// 		logger.log(LogLevel::ERROR, "Usage: % [config_path]", argv[0]);
// 		return (EXIT_FAILURE);
// 	}
// 	HTTPServer server(argc == 2 ? argv[1] : "config/default.conf");
// 	return (server.run());
// }

// have a html page												--> done
// create fast links for the files / dirs						--> 
// be able to go inside dirs and navigate back and forth		--> 

#include <iostream>
#include <filesystem>

void generateDirectoryListing(const std::string& directoryPath) {
    std::cout << "Content-Type: text/html\n\n";
    std::cout << "<html><head><title>Index of " << directoryPath << "</title></head><body>\n";
    std::cout << "<h1>Index of " << directoryPath << "</h1><hr><ul>\n";

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        std::cout << "<li><a href=\"" << entry.path().filename() << "\">" << entry.path().filename() << "</a></li>\n";
    }

    std::cout << "</ul><hr></body></html>\n";
}

int main() {
    // ... handle incoming requests

    // Example: If requested path corresponds to a directory without an index file
    std::string requestedPath = "../data/";
    // if (std::is_directory(requestedPath)) {
        generateDirectoryListing(requestedPath);
    // }

    // ... continue handling other requests

    return 0;
}
