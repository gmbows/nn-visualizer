#include <iostream>
#include "Neuron.h"
#include <random>
#include <vector>
#include <ctime> 
#include <cmath>
#include <fstream>
#include <utility>
#include <SDL2/SDL.h>
#include "Window.h"

int main(int argc, char** argv) {
	
	srand(time(NULL));
	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	Window window = Window();
	
	Network *net = new Network(window.topology);
	
	unsigned trials = 0;
	
	while(window.running and window.has_samples()) {
		if(window.pause) {
			window.update(net);
			continue;
		}
		std::pair<std::vector<float>,std::vector<float>> sample = window.training_data.front();
		window.training_data.pop();
		
		std::vector<float> input = sample.first;
		std::vector<float> output = sample.second;
		
		//Get result and back prop target value
		net->feed_forward(input);

		net->back_prop(output);
		
		int a = 0;
		if(output.size() == 1) {
			a = output.at(0);
		} else {
			for(int i=0;i<output.size();i++) {
				if(output.at(i) == 1) a = i;
			}
		}
		
		window.stage = std::to_string(a);
		
		//Update GUI
		window.update(net);
		
		window.trials = ++trials;
	}
	
	std::cout << "Done training" << std::endl;

	while(window.running) {
		window.update(net);
		continue;
	}

	SDL_Quit();
	return 0;
}