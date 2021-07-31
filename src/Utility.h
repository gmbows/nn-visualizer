#pragma once

#include <vector>
#include <queue>

template <typename T>
bool contains(std::vector<T> v, T t) {
	for(auto &e : v) {
		if(e == t) return true;
	}
	return false;
}

template <typename T>
std::vector<T> remove_dupes(std::vector<T> v) {
	std::vector<T> passed;
	std::vector<T> uniq;
	for(auto &e : v) {
		if(!contains(passed,e)) {
			uniq.push_back(e);
			passed.push_back(e);
		}
	}
	return uniq;
}

template <typename T>
std::vector<T> swap(std::vector<T> v,int i,int j) {
	T temp = v.at(j);
	v.at(j) = v.at(i);
	v.at(i) = temp;
	return v;
}

std::vector<std::vector<unsigned int>> permute(std::vector<unsigned int>);

std::vector<std::vector<unsigned int>> network_permute(unsigned int xmax, unsigned int ymax);

std::string import_file(const std::string &filename);

std::vector<std::string> split(std::string s, char token);

template <class T>
struct LimitedQueue {
	int size;
	
	std::queue<T> queue;
	
	LimitedQueue(int s): size(s) {
	
	}
	
	std::vector<T> get_elements() {
		std::vector<T> elements;
		std::queue<T> copy = this->queue;
		for(int i=0;i<copy.size();i++) {
			elements.push_back(copy.front());
			copy.pop();
		}
		return elements;
	}
	
	T pop() {
		return this->queue.front();
		this->queue.pop();
	}
	
	void push(T t) {
		this->queue.push(t);
		if(this->queue.size() == size) {
			this->queue.pop();
		}
	}
	
	double average() {
		double sum = 0.0;
		for(auto &e : this->get_elements()) {
			sum += e;
		}
		
		return sum/this->get_elements().size();
	}
	
};