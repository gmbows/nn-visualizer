#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <cmath>

float random_weight();
float sigmoid(float n);

float dsigmoid(float n);

struct Layer;

extern unsigned int num_neurons;

struct Neuron {
	unsigned int id;
	bool bias;
	float value;
	float gradient;
	float eta = .15;
	float alpha = .3;
	float sumDOW(Layer *next);
	float(*transform)(float) = sigmoid;
	float(*dtransform)(float) = dsigmoid;
	std::map<Neuron*,float> weights;
	std::map<Neuron*,float> delta_weights;
	
	void calchgrad(Layer* next);
	void calc_gradient(float target);
	void update_input_weights(Layer *prev);
	
	void feed_forward(Layer* prevlayer);
	
	Neuron(float init) {
		this->value = init;
		this->id = num_neurons++;
	}
};

struct Layer {
	std::vector<Neuron*> neurons;
	int size;
	
	Layer(unsigned int _size): size(_size) {
		//<= for bias neuron
		for(int i=0;i < this->size;++i) {
			Neuron *n = new Neuron(random_weight());
			n->bias = false;
			this->neurons.push_back(n);
		}
		this->neurons.back()->value = 1.0;
		this->neurons.back()->bias = true;
	}
};

struct Network {
	
	float err;
	
	std::vector<Layer*> layers;

	void feed_forward(std::vector<float> values);
	std::vector<float> get_results();
	
	void back_prop(std::vector<float> values);
	void get_results(std::vector<float> values);
	
	unsigned int size;

	Network(std::vector<unsigned int> topology): size(topology.size()) {
		unsigned short bias = 0;
		for(int i=0;i<topology.size();++i ) {
			bias = (i != topology.size()-1);
			this->layers.push_back(new Layer(topology.at(i)+bias));
		}
		unsigned init = this->init_weights();
		std::cout << "Initialized " << init << " weights" << std::endl;
		std::cout << "Created network with following topology: " << std::flush;
		for(auto &layer : this->layers) {
			std::cout << layer->size << " " << std::flush;
		}
		std::cout << std::endl;
	}
	
	unsigned int init_weights();
	
};
