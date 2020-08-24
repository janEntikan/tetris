#include "fileio.h"
#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>


std::map<std::string, std::vector<int>> load_dat(std::string path) {
	std::map<std::string, std::vector<int>> content;
    content["none"] = {0};
	std::ifstream input(path);
	for(std::string line; getline(input, line);) {
		if (!(line == "")) {
			std::istringstream iss(line);
			std::vector<std::string> results(
				std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>()
			);
			content[results[0]] = {};
			for (int i = 0; i<results.size()-1; i++) {
				int frame = atol(results[i+1].c_str());
				content[results[0]].push_back(frame);
			}
		}
	}
	return content;
}
