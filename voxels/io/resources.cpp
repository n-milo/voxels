#include "resources.h"

#include <iostream>
#include <fstream>
#include <exception>

std::string io::getFileContents(std::string filename) {
	std::ifstream file("/Users/nikola/Documents/Xcode Projects/ae2/ae2/resources/" + filename);
	
	if (file.fail()) {
		file.close();
		std::string msg = "IO error: ";
		msg += strerror(errno);
		throw std::runtime_error(msg);
	}
	
	std::string buffer;
	std::string line;
	while (file) {
		std::getline(file, line);
		buffer += (line + "\n");
	}
	file.close();
	
	return buffer;
}
