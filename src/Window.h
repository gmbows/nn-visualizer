#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include <utility>
#include <iostream>
#include <queue>
#include "Neuron.h"
#include <string>

std::string truncate(float n);

struct Window {
	
	SDL_Renderer *renderer;
	SDL_Window *window;
	
	SDL_Surface *textSurface;
	
	int height;
	int width;
	
	bool pause = false;
	bool show_gradient = false;
	
	int tickrate = 5;
	
	std::string stage;
	
	std::vector<unsigned int> topology;
	
	std::queue<std::pair<std::vector<float>,std::vector<float>>> training_data;
	
	void import_training_data(std::string);
	
	bool inline has_samples() {
		return this->training_data.size() > 0;
	}
	
	TTF_Font* font;
	int font_size = 44;
	SDL_Color fgColor;
	SDL_Color bgColor;
	
	Network* net;

	bool running = false;
	bool mousedown = false;
	bool rightmousedown = false;
	
	void create_window();
	
	int trials = 0;
	
	void draw_network();
	void draw_neuron(Neuron*,int x,int y);
	void draw_circle(int,int,int,Uint8,Uint8,Uint8,Uint8);
	void draw_text(std::string str,int x,int y);
	void draw_text(float f,int x,int y) {
		this->draw_text(truncate(f),x,y);
	}
	
	void input();
	
	//take input and refresh screen
	void update(Network*);
	
	Window();
	
};