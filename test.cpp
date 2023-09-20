#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void	post_data(const std::string filename, const std::string post_data) 
{
	std::ifstream		inputFile(filename);
	std::ofstream		outputFile("temp.txt");
	std::ofstream		outData(post_data)
	const std::string	check_point = "        <!-- IMAGES -->";
	std::string			converted_post_data = "        <img src=\"/home/lvan-bus/Documents/webserv/data/";
	bool				done = true;

	if (!inputFile || !outputFile) {
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

int main(void) {
    // const std::string filename = "example.txt";
	const std::string filename = "data/www/index.html";
    const std::string   post_message = "nose_monkey.webp";
    post_data(filename, post_message);
	return 0;
}