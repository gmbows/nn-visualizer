#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <cmath>
#include <queue>

#include "Utility.h"

double random_weight();

double h_tan(double n);
double dh_tan(double n);

double sigmoid(double n);
double d_sigmoid(double n);

double relu(double n);
double d_relu(double n);

double l_relu(double n);
double dl_relu(double n);


struct Layer;

extern unsigned int num_neurons;

extern double _eta;
extern double _alpha;

struct Neuron {
	unsigned int id;
	bool bias;
	double value;
	double gradient;
	double sumDOW(Layer *next);
	double(*transform)(double) = h_tan;
	double(*dtransform)(double) = dh_tan;
	std::map<Neuron*,double> weights;
	std::map<Neuron*,double> delta_weights;
	
	void calchgrad(Layer* next);
	void calc_gradient(double target);
	void update_input_weights(Layer *prev,float,float);
	
	void feed_forward(Layer* prevlayer);
	
	Neuron(double init) {
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
	
	std::queue<std::pair<std::vector<double>,std::vector<double>>> training_data;
	
	bool valid;
	
	void import_training_data(std::string);
	void import_params(std::string);
	
	bool inline has_samples() {
		return this->training_data.size() > 0;
	}
	
	double err;
	LimitedQueue<double> error_history = LimitedQueue<double>(1000);
	
	std::vector<Layer*> layers;

	void feed_forward(std::vector<double> values);
	std::vector<double> get_results();
	
	void back_prop(std::vector<double> values);
	void get_results(std::vector<double> values);
	
	std::vector<unsigned int> topology;
	
	unsigned int size;
	
	double eta = _eta;
	double alpha = _alpha;
	
	void generate() {
		unsigned short bias = 0;
		for(int i=0;i<this->topology.size();++i ) {
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
		std::cout << "Created network with following topology (including bias neurons): " << std::flush;
		this->valid = true;
		for(auto &layer : this->layers) {
			std::cout << layer->size << " " << std::flush;
		}
		std::cout << std::endl;
	}
	
	Network(std::vector<unsigned int> top) {
		this->topology = top;
		this->size = this->topology.size();
		this->import_training_data("training_data.txt");
	}
	

	Network() {
		this->import_params("params.txt");
		this->import_training_data("training_data.txt");
		this->generate();
	}
	
	unsigned int init_weights();
	
};
