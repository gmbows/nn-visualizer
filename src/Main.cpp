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

 std::vector<unsigned> run_tests() {
	 //Test all network topologies of size tx_max and layer size ty_max
	 //Return topology of network that reached err_threshold in the fewest trials
	unsigned int input_layer_size = 32;
	unsigned int output_layer_size = 6;
	unsigned int tx_max = 3;
	unsigned int ty_max = 32;

	std::vector<std::vector<unsigned int>> tops = remove_dupes(network_permute(tx_max,ty_max));

	double err_threshold = 0.05;
	double min_trials = 10;
	int max_trials = 24000;
	int best_trial = 50000;
	int trials;
	std::vector<unsigned int> best_top;
	
	Network test_net = Network();
	test_net.import_training_data("training_data.txt");
	std::queue<std::pair<std::vector<double>,std::vector<double>>> training_set = test_net.training_data;
	
	for(auto &top : tops) {
		trials = 0;
		std::vector<unsigned int> test_top = top;
		test_top.emplace(test_top.begin(),input_layer_size);
		test_top.push_back(output_layer_size);
		std::cout << "Testing topology " << std::flush;
		for(auto &l : test_top) std::cout << l << " " << std::flush;
		std::cout << std::endl;
		Network *net = new Network(test_top);
		std::queue<std::pair<std::vector<double>,std::vector<double>>> this_training_set = training_set;
		while(this_training_set.size() > 0) {
			std::pair<std::vector<double>,std::vector<double>> sample = this_training_set.front();
			this_training_set.pop();
			
			std::vector<double> input = sample.first;
			for(int i=0;i<input.size();i++) {
				if(input.at(i) == 0) input.at(i) = -1;
			}
			std::vector<double> output = sample.second;
			// std::cout << "hi" << std::endl;
			net->feed_forward(input);
			net->back_prop(output);
			trials++;
			if(trials > max_trials) break;
			if(net->err < err_threshold and trials > min_trials) {
				if(trials < best_trial) {
					std::cout << "New best at " << trials << " trials "<<std::endl;
					best_trial = trials;
					best_top = test_top;
					break;
				}
			}
		}
	}
	std::cout << "finished testing" << std::endl;
	for(auto &l : best_top) std::cout << l << std::flush;
	std::cout << std::endl;
	std::cout << "trials: " << best_trial << std::endl;
	return best_top;
}

int main(int argc, char** argv) {
	
	srand(time(NULL));
		
	// std::vector<unsigned> top = run_tests();
	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	
	// if(top.size() == 0) {
		// std::cout << "Topology optimization failed, exiting" << std::endl;
		// return 0;
	// }
	
	Network *net = new Network();
	// Network *net = new Network(top);
	// net->import_training_data("training_data.txt");
	// net->topology = top;
	// net->size = top.size();
	
	Window window = Window(net);
	
	unsigned trials = 0;
	
	std::vector<std::string> learning_data;
	
	while(window.running and net->has_samples() and net->valid) {
		if(window.pause) {
			window.update();
			continue;
		}
		std::pair<std::vector<double>,std::vector<double>> sample = net->training_data.front();
		net->training_data.pop();
		
		std::vector<double> input = sample.first;
		std::vector<double> output = sample.second;

		
		//Get result and back prop target value
		net->feed_forward(input);

		net->back_prop(output);
		
		if(trials%100 == 0) learning_data.push_back(truncate(net->err));
		
		write_vector_to_file(learning_data,"training.txt");
		
		std::string a;
		if(output.size() == 1) {
			a = std::to_string(output.at(0));
		} else {
			for(int i=0;i<output.size();i++) {
				a += std::to_string((int)output.at(i));
			}
		}
		
		window.stage = a;
		
		//Update GUI
		if(trials%100 == 0) window.update();
		
		window.trials = ++trials;
	}
	
	std::cout << "Done training" << std::endl;

	while(window.running and net->valid) {
		window.update();
		continue;
	}

	SDL_Quit();
	return 0;
}