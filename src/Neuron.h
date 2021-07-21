#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <cmath>
#include <queue>

float random_weight();

float h_tan(float n);
float dh_tan(float n);

float sigmoid(float n);
float d_sigmoid(float n);

float relu(float n);
float d_relu(float n);

float l_relu(float n);
float dl_relu(float n);


struct Layer;

extern unsigned int num_neurons;

extern float _eta;
extern float _alpha;

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
	
};

struct Neuron {
	unsigned int id;
	bool bias;
	float value;
	float gradient;
	float sumDOW(Layer *next);
	float(*transform)(float) = h_tan;
	float(*dtransform)(float) = dh_tan;
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
			Neuron *n = new Neuron(0.0);
			n->bias = false;
			this->neurons.push_back(n);
		}
	}
};

struct Network {
	
	float err;
	LimitedQueue<float> error_history = LimitedQueue<float>(100);
	
	std::vector<Layer*> layers;

	void feed_forward(std::vector<float> values);
	std::vector<float> get_results();
	
	void back_prop(std::vector<float> values);
	void get_results(std::vector<float> values);
	
	unsigned int size;
	
	float eta = _eta;
	float alpha = _alpha;
	

	Network(std::vector<unsigned int> topology): size(topology.size()) {
		unsigned short bias = 0;
		for(int i=0;i<topology.size();++i ) {
			bias = (i < topology.size()-2);
			Layer *new_layer = new Layer(topology.at(i)+bias);
			if(bias) {
				new_layer->neurons.back()->bias = true;
				new_layer->neurons.back()->value = 1.0;
			}
			this->layers.push_back(new_layer);
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
