#include "Neuron.h"
#include "Utility.h"

#include <cmath>

unsigned int num_neurons = 0;
double _eta = .04;
double _alpha = .1;

double random_weight() {
	double w =  (rand() / (double)RAND_MAX)-(rand() / (double)RAND_MAX);
	// double w =  (rand() / (double)RAND_MAX);
	// std::cout << w << std::endl;
	return w;
}

double h_tan(double n) {
	return tanh(n);
}

double dh_tan(double n) {
	return (1 / cosh(n))*(1 / cosh(n));
}

double sigmoid(double n) {
	// return 1/(1+(pow(M_E,-n)));
	return (1/(1+(pow(M_E,-n*3))))-.5;
}

double d_sigmoid(double n) {
	// return sigmoid(n)*(1-sigmoid(n));
	double num = pow((3*M_E),-3*n);
	double d = pow(pow(M_E,-3*n)+1,2);
	return num/d;
}

double relu(double n) {
	return std::max(0.0,n);
}

double d_relu(double n) {
	if(n <= 0) return 0;
	return 1;
}

double l_relu(double n) {
	return std::max(0.1*n,n);
}

double dl_relu(double n) {
	if(n <= 0) return 0.1;
	return 1;
}

void Neuron::feed_forward(Layer* prevlayer) {
	double sum = 0.0;
	if(this->bias) return;
	for(auto &neuron : prevlayer->neurons) {
		if(neuron->weights.find(this) != neuron->weights.end()) {
			sum += neuron->value * neuron->weights.at(this);
		} else {
			std::cout << "Neuron::feed_forward(): Neuron weight not found" << std::endl;
		}
	}
	this->value = this->transform(sum);
}

void Network::feed_forward(std::vector<double> values) {
	if(values.size() != this->layers.at(0)->size - 1) {
		std::cout << "Network::feed_forward(): Invalid input size (" << values.size() << ", " <<  this->layers.at(0)->size - 1 << ")" << std::endl;
		this->valid = false;
		return;
	}
	// std::cout << "Network::feed_forward(): Feeding forward" << std::endl;
	//Feed input values into input layer
	for(int i=0;i<values.size();i++) {
		this->layers.at(0)->neurons.at(i)->value = values.at(i);
	}
	
	//Feed each non-input layer forward
	for(int i=1;i<layers.size();i++) {
		for(auto &neuron : this->layers.at(i)->neurons) {
			neuron->feed_forward(this->layers.at(i-1));
		}
	}
}

double Neuron::sumDOW(Layer *next) {
	double sum = 0.0f;
	for(int i=0;i<next->size;i++) {
		Neuron *neuron = next->neurons.at(i);
		sum += this->weights.at(neuron)*neuron->gradient;
	}
	return sum;
}

void Neuron::update_input_weights(Layer *prev,float eta,float alpha) {
	for(auto &neuron : prev->neurons) {
		
		//Calculate delta weight and update connection weight
		double old_delta = neuron->delta_weights.at(this);
		double newd = eta * neuron->value * this->gradient + alpha * old_delta;
		neuron->delta_weights.at(this) = newd;
		
		neuron->weights.at(this) += newd;
	}
}

void Neuron::calc_gradient(double target) {
	double err = target - this->value;
	this->gradient = err*this->dtransform(this->value);
}

void Neuron::calchgrad(Layer* next) {
	double d = sumDOW(next);
	// if(d == 0) std::cout << "Neuron " << this->id << " has 0 gradient" << std::endl;
	this->gradient = d * this->dtransform(this->value);
}

void Network::back_prop(std::vector<double> target_values) {
	Layer *output_layer = this->layers.back();
	this->err = 0.0f;
	
	if(target_values.size() != output_layer->size) {
		std::cout << "Network::back_prop(): Target values list size does not match output layer size (got " << target_values.size() << ", expected " << output_layer->size << ")" << std::endl;
		this->valid = false;
		return;
	}
	
	//Mean squared error for output layer
	
	for(int i=0;i<output_layer->size;i++) {
		double d = target_values.at(i) - output_layer->neurons.at(i)->value;
		this->err += d*d;
	}
	
	this->err /= output_layer->size;
	this->err = sqrt(this->err);
	
	this->error_history.push(err);
		
	this->err = this->error_history.average();
		
	//Calculate gradient for each output neuron
	for(int i=0;i<output_layer->size;i++) {
		output_layer->neurons.at(i)->calc_gradient(target_values.at(i)); 
	}
	
	for(int layernum = this->size-2;layernum > 0; --layernum) {
		Layer *layer = this->layers.at(layernum);
		Layer *next = this->layers.at(layernum+1);
		
		// std::cout << "Calculating hidden gradient for layer " << layernum << std::endl;
		for(auto &neuron : layer->neurons) {
			neuron->calchgrad(next);
		}
	}
	
	for(int i=this->size-1;i > 0;--i) {
		Layer *layer = this->layers.at(i);
		Layer *prev = this->layers.at(i-1);
		for(auto &neuron : layer->neurons) {
			neuron->update_input_weights(prev,this->eta,this->alpha);
		}
	}
}

std::vector<double> Network::get_results() {
	std::vector<double> results;
	for(int i=0;i<this->layers.back()->size;i++) {
		results.push_back(this->layers.back()->neurons.at(i)->value);
	}
	return results;
}

unsigned int Network::init_weights() {
	unsigned int total=0;
	if(this->size <= 1) {
		std::cout << "Network::init_weights(): Invalid network topology" << std::endl;
		return 0;
	}
	for(int i=0;i<this->size-1;i++) {
		total += this->layers.at(i)->size;
		for(auto &Lneuron : this->layers.at(i)->neurons) {
			for(auto &Rneuron : this->layers.at(i+1)->neurons) {
				if(Rneuron->bias) {
					Lneuron->weights.insert({Rneuron,0.0});
				} else {
					Lneuron->weights.insert({Rneuron,random_weight()});
				}
				Lneuron->delta_weights.insert({Rneuron,0.0});
			}
		}
	}
	return total;
}

void Network::import_training_data(std::string filename) {
	std::string raw = import_file(filename);
	std::vector<std::string> lines = split(raw,'\n');
	for(auto &line : lines) {
		std::vector<double> input,output;
		std::string in_string,out_string;
		try {
			in_string = split(line,'|').at(0);
			out_string = split(line,'|').at(1);
		} catch(const std::exception &e ){
			std::cout << line << std::endl;
		}
		std::vector<std::string> in_values = split(in_string,' ');
		std::vector<std::string> out_values = split(out_string,' ');
		for(auto &v : in_values) {
			try {
				input.push_back(std::stof(v));
			} catch(const std::exception &e ){
					std::cout << v << std::endl;
			}
		}
		for(auto &v : out_values) {
			try {
				output.push_back(std::stof(v));
			} catch(const std::exception &e ){
				std::cout << v << std::endl;
			}
		}
		this->training_data.push(std::make_pair(input,output));
	}
	std::cout << "Imported " << this->training_data.size() << " training samples" << std::endl;
}

void Network::import_params(std::string filename) {
	std::string raw = import_file(filename);
	std::vector<std::string> lines = split(raw,'\n');
	std::string top = lines.at(0);
	std::vector<std::string> vtop = split(top,' ');
	for(auto &c : vtop) {
		this->topology.push_back(std::stoi(c));
	}
	this->size = this->topology.size();
	this->eta = std::stof(split(lines.at(1),' ').at(0));
	this->alpha = std::stof(split(lines.at(1),' ').at(1));
	std::cout << "Imported network params" << std::endl;
}