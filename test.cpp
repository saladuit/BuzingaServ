#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void	post_image(const std::string filename, const std::string post_data) 
{
	std::ifstream		inputFile(filename);
	std::ofstream		outputFile("temp.txt");
	std::ifstream		image("data/" + post_data);
	const std::string	check_point = "        <!-- IMAGES -->";
	std::string			converted_post_data = "        <img src=\"/home/lvan-bus/Documents/webserv/data/";
	bool				done = true;

	if (!inputFile || !outputFile || !image) {
		std::cerr << "Error opening files." << std::endl;
		return ;
    }
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
        return ;
    }

    std::cout << "Text inserted successfully." << std::endl;
}


void	post_text(const std::string filename, const std::string content)
{
	std::ifstream		inputFile(filename);
	std::ofstream		outputFile("temp.txt");
	const std::string	check_point = "        <!-- text/plain -->";
	std::string			converted_post_data = "        <P ALIGN=\"LEFT\">";
	bool				done = true;

	if (!inputFile || !outputFile) {
		std::cerr << "Error opening files." << std::endl;
		return ;
    }
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
        return ;
    }

    std::cout << "Text inserted successfully." << std::endl;
}

// when implementing save_image it can be simplified by replacing
// function body with:
	// std::ofstream	new_image("data/" + post_message);
	// new_image << conent;
void	save_image(const std::string post_message, const std::string content)
{
	std::ifstream	image_content(content);
	std::string		line;

	if (!image_content)
	{
		std::cerr << "Error opening files." << std::endl;
		return ;
	}
	std::ofstream	new_image("data/" + post_message);
	while (std::getline(image_content, line)) {
    	new_image << line << '\n';
    }
}

int main(int argc, char **argv) {
	if (argc != 4)
	{
		std::cout << "Invalid number of parameters.\n";
		return 0;
	}

	const std::string	filename = "data/www/index.html";
    const std::string   post_message = argv[2];
	const std::string	content_type = argv[1];
	if (content_type == "image") {
		save_image(post_message, argv[3]);
		post_image(filename, post_message);
	}
	else if (content_type == "text")
		post_text(filename, post_message);
	return 0;
}