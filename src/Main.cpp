#include <iostream>
#include "Neuron.h"
#include <random>
#include <ctime> 
#include <cmath>
#include <fstream>
#include <SDL2/SDL.h>
#include "Window.h"

std::string truncate(float n) {
	std::string s = std::to_string(n);
	return s.substr(0,std::min((int)s.size(),4));
}

int main(int argc, char** argv) {
	
	srand(time(NULL));
	
	Network *net = new Network({2,4,1});
				
	float p,x,r,q;
	unsigned i = 0;
	float err = 1;
	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	Window window = Window(1000,800);
	
	while(err > 0.000 and window.running) {
		float p=rand()%2;
		float r=rand()%2;
		float x=rand()%2;
		float q = p and r;
		

		net->feed_forward({p,r});
		net->back_prop({q});
		std::vector<float> values;
		for(auto &layer : net->layers) {
			for(auto &neuron : layer->neurons) {
				values.push_back(neuron->value);
			}
		}
		values.push_back(net->err);
		for(auto &val : values) {
			std::cout << truncate(val) << "    " << std::flush;
		}
		std::cout << std::endl;
		// std::getline(std::cin,wait);
		// net->back_prop({q});
		err = net->err;
		window.value = err;
		if(i%50 == 0) window.update();
		i++;
	}
	// net->feed_forward({p,r,x});
	std::cout << "EXPECTED " << q << std::endl;
	std::cout << "RESULT " <<  std::flush;
	for(auto &i : net->get_results()) {
		std::cout << i  << " "<< std::flush;
	}
	// net->back_prop({q});
	std::cout << std::endl;
	std::cout << "ERROR " << net->err << std::endl;
	std::cout << "Took " << i << " trials" << std::endl;
	std::cout << std::endl;
	
	std::cout << SDL_GetError() << std::endl;
	SDL_Quit();
	return 0;
}