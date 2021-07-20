#include <iostream>
#include "Neuron.h"
#include <random>
#include <vector>
#include <ctime> 
#include <cmath>
#include <fstream>
#include <SDL2/SDL.h>
#include "Window.h"

int main(int argc, char** argv) {
	
	srand(time(NULL));
	Network *net = new Network({4,4,3,6,1});
	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	Window window = Window(1200,1000);
	
	float p,x,r,q,w;
	unsigned i = 0;
	float err = 1;
	
	while(err > 0.000 and window.running) {
		p=rand()%2;
		r=rand()%2;
		x=rand()%2;
		w=rand()%2;
		q = (p and r) ^ (x and w);

		net->feed_forward({p,r,x,w});
		net->back_prop({q});
		
		std::vector<float> values;
		for(auto &layer : net->layers) {
			for(auto &neuron : layer->neurons) {
				values.push_back(neuron->value);
			}
		}
		
		values.push_back(net->err);
		for(auto &val : values) {
			// std::cout << truncate(val) << "    " << std::flush;
		}
		std::cout << std::endl;

		err = net->err;
		if(i%50 == 0) window.update(net);
		window.trials = i;
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
	
	// std::cout << SDL_GetError() << std::endl;
	SDL_Quit();
	return 0;
}