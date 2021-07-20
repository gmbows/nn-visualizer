#include "Neuron.h"

unsigned int num_neurons = 0;

float random_weight() {
	return rand() / (float)RAND_MAX;
}

float sigmoid(float n) {
	return tanh(n);
}

float dsigmoid(float n) {
	return (1 / cosh(n))*(1 / cosh(n));
}

void Neuron::feed_forward(Layer* prevlayer) {
	float sum = 0.0f;
	// if(this->bias) return;
	for(auto &neuron : prevlayer->neurons) {
		if(neuron->weights.find(this) != neuron->weights.end()) {
			sum += neuron->value * neuron->weights.at(this);
		} else {
			std::cout << "Neuron::feed_forward(): Neuron weight not found" << std::endl;
		}
	}
	this->value = this->transform(sum);
}

void Network::feed_forward(std::vector<float> values) {
	if(values.size() != this->layers.at(0)->size - 1) {
		std::cout << "Network::feed_forward(): Invalid input size (" << values.size() << ", " <<  this->layers.at(0)->size - 1 << ")" << std::endl;
		return;
	}
	// std::cout << "Network::feed_forward(): Feeding forward" << std::endl;
	for(int i=0;i<values.size();i++) {
		this->layers.at(0)->neurons.at(i)->value = values.at(i);
	}
	
	for(int i=1;i<layers.size();i++) {
		for(auto &neuron : this->layers.at(i)->neurons) {
			float bef = neuron->value;
			neuron->feed_forward(this->layers.at(i-1));
			// std::cout << "(" << bef << ", " << neuron->value << ")" << std::endl;
	
		}
	}
}

float Neuron::sumDOW(Layer *next) {
	float sum = 0.0f;
	for(int i=0;i<next->size;i++) {
		Neuron *neuron = next->neurons.at(i);
		sum += this->weights.at(neuron)*neuron->gradient;
	}
	return sum;
}

void Neuron::update_input_weights(Layer *prev) {
	for(auto &neuron : prev->neurons) {
		float old = neuron->delta_weights.at(this);
		float newd = eta * neuron->value * this->gradient + alpha * old;
		neuron->delta_weights.at(this) = newd;
		// neuron->weights.at(this) += newd;
		
		float oldw = neuron->weights.at(this);
		neuron->weights.at(this) += newd;
		float neww = neuron->weights.at(this);
	}
}

void Neuron::calc_gradient(float target) {
	float err = target - this->value;
	this->gradient = err*this->dtransform(this->value);
}

void Neuron::calchgrad(Layer* next) {
	float d = sumDOW(next);
	// if(d == 0) std::cout << "Neuron " << this->id << " has 0 gradient" << std::endl;
	this->gradient = d * this->dtransform(this->value);
}

void Network::back_prop(std::vector<float> target_values) {
	Layer *output_layer = this->layers.back();
	this->err = 0.0f;
	
	if(target_values.size() != output_layer->size) {
		std::cout << "Network::back_prop(): Target values list size does not match output layer size (" << target_values.size() << ", " << output_layer->size << ")" << std::endl;
		return;
	}
	
	//Mean squared error for output layer
	
	for(int i=0;i<output_layer->size;i++) {
		float d = target_values.at(i) - output_layer->neurons.at(i)->value;
		this->err += d*d;
	}
	
	this->err /= output_layer->size;
	this->err = sqrt(this->err);
	
	this->error_history.push(err);
	
	float sum = 0.0;
	for(auto &e : this->error_history.get_elements()) {
		sum += e;
	}
	
	this->err = sum/this->error_history.get_elements().size();
		
	//Calculate gradient for each output neuron
	for(int i=0;i<output_layer->size;i++) {
		output_layer->neurons.at(i)->calc_gradient(target_values.at(i)); 
	}
	
	for(int layernum = this->size-2;layernum > 0; --layernum) {
		Layer *hidden = this->layers.at(layernum);
		Layer *next = this->layers.at(layernum+1);
		
		// std::cout << "Calculating hidden gradient for layer " << layernum << std::endl;
		for(auto &neuron : hidden->neurons) {
			neuron->calchgrad(next);
		}
	}
	
	for(int i=this->size-1;i > 0;--i) {
		Layer *layer = this->layers.at(i);
		Layer *prev = this->layers.at(i-1);
		for(auto &neuron : layer->neurons) {
			neuron->update_input_weights(prev);
		}
	}
}

std::vector<float> Network::get_results() {
	std::vector<float> results;
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
				Lneuron->weights.insert({Rneuron,random_weight()});
				Lneuron->delta_weights.insert({Rneuron,0.0});
			}
		}
	}
	return total;
}