#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int	post_image(const std::string filename, const std::string post_data) 
{
	std::ifstream		inputFile(filename);
	std::ofstream		outputFile("temp.txt");
	std::ifstream		image("data/images/" + post_data);
	const std::string	check_point = "        <!-- image -->";
	std::string			converted_post_data = "        <img src=\"/home/lvan-bus/Documents/webserv/data/images/";
	bool				done = true;

	if (!inputFile || !outputFile || !image) {
		std::cerr << "Error opening files." << std::endl;
		return 1;
    }
	if (image.peek() == std::string::traits_type::eof())
		return 204;
	converted_post_data += post_data + "\" width=250 height=200>";
	std::string line;
	std::vector<std::string> lines;
	int currentLine = 0;
	while (std::getline(inputFile, line)) {
		currentLine++;
		if (line == check_point && done) {
			lines.push_back(line);
			lines.push_back(converted_post_data);
			done = false;
			continue ;
		}
		lines.push_back(line);
	}
    for (const std::string& storedLine : lines) {
        outputFile << storedLine << '\n';
    }
    inputFile.close();
    outputFile.close();
    if (std::rename("temp.txt", filename.c_str()) != 0) {
        std::cerr << "Error renaming the file." << std::endl;
        return 1;
    }
    std::cout << "Image inserted successfully." << std::endl;
	return 0;
}


int	post_text(const std::string filename, const std::string content)
{
	std::ifstream		inputFile(filename);
	std::ofstream		outputFile("temp.txt");
	const std::string	check_point = "        <!-- text -->";
	std::string			converted_post_data = "        <P ALIGN=\"LEFT\">";
	bool				done = true;

	if (!inputFile || !outputFile) {
		std::cerr << "Error opening files." << std::endl;
		return 1;
    }
	if (content == "")
		return 204;
	converted_post_data += content + "</P>";
	std::string line;
	std::vector<std::string> lines;
	int currentLine = 0;
	while (std::getline(inputFile, line)) {
		currentLine++;
		if (line == check_point && done) {
			lines.push_back(line);
			lines.push_back(converted_post_data);
			done = false;
			continue ;
		}
		lines.push_back(line);
	}

    for (const std::string& storedLine : lines) {
        outputFile << storedLine << '\n';
    }

    inputFile.close();
    outputFile.close();

    if (std::rename("temp.txt", filename.c_str()) != 0) {
        std::cerr << "Error renaming the file." << std::endl;
        return 1;
    }

    std::cout << "Text inserted successfully." << std::endl;
	return 0;
}

// when implementing save_image it can be simplified by replacing
// function body with:
	// std::ofstream	new_image("data/" + post_message);
	// new_image << conent;
bool	save_data(const std::string post_message, const std::string content, 
			const std::string content_type)
{
	std::ifstream	image_content(content);
	std::string		line;

	if (!image_content)
	{
		std::cerr << "Error opening files." << std::endl;
		return (false);
	}
	// std::cout << "data/" + content_type + "/" + post_message + "\n";
	std::ofstream	new_image("data/" + content_type + "/" + post_message);
	while (std::getline(image_content, line)) {
    	new_image << line << '\n';
    }
	return (true);
}

int main(int argc, char **argv) 
{
	if (argc != 4)
	{
		std::cout << "Invalid number of parameters.\n";
		return 0;
	}
	// const std::string	filename = "data/www/index.html";
    const std::string   post_message = argv[2];
	const std::string	content_type = argv[1];
	if (save_data(post_message, argv[3], content_type) == false) {
		std::cout << "400 BAD REQUEST\n";
		return (400);
	}

	// post functionality is optional I guess
	// if (content_type == "images") {
	// 	if (post_image(filename, post_message) == 204) {
	// 		std::cout << "204 NO CONTENT\n";
	// 		return (204);
	// 	}
	// } else if (content_type == "text") {
	// 	if (post_text(filename, post_message) == 204) {
	// 		std::cout << "204 NO CONTENT\n";
	// 		return (204);
	// 	}
	// }
	return 0;
}