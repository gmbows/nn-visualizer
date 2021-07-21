#include <iostream>
#include <fstream>

#include "Utility.h"

std::string import_file(const std::string &filename) {
	std::string contents;
	std::ifstream f(filename,std::ios::binary);
	if(!f.is_open()) return "";
	char c;
	while(f >> std::noskipws >> c) {
		contents += c;
	}
	return contents;
}
	
std::vector<std::string> split(std::string s, char token) {
	std::vector<std::string> v;
	std::string temp;
	for(auto &c : s) {
		if(c == token) {
			v.push_back(temp);
			temp = "";
		} else {
			temp += c;
		}
	}
	v.push_back(temp);
	return v;
}
