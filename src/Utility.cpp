#include <iostream>
#include <fstream>
#include <algorithm>

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

std::vector<std::vector<unsigned int>> permute(std::vector<unsigned int> v) {
	std::vector<std::vector<unsigned int>> p;
	std::sort(v.begin(),v.end());
	p.push_back(v);
	while(std::next_permutation(v.begin(),v.end())) {
		p.push_back(v);
	}
	return p;
}

std::vector<std::vector<unsigned int>> network_permute(unsigned int xmax, unsigned int ymax) {
	//Returns a vector of vectors that contain every permutation of values less than or equal to xmax of length less than or equal to ymax
	//Or every possible network topology that fits in the dimensions xmax, ymax
	std::vector<std::vector<unsigned int>> perms;	
	
	const int min_neurons = 2;
	
	std::vector<unsigned int> top;
	for(unsigned int x = 0;x<xmax;x++) {
		top = {};
		for(int i=0;i<x;i++) {
			top.push_back(min_neurons);
		}
		for(int i=0;i<top.size();i++) {
			for(unsigned int y = min_neurons;y<=ymax;y++) {
				top.at(i) = y;
				for(auto &v : permute(top)) {
					perms.push_back(v);
				}
			}
		}

	}
	for(auto &v : perms) {
		for(auto &l : v) {
			// std::cout <<  l << std::flush;
		}
		// std::cout << std::endl;
	}
	return perms;
}
