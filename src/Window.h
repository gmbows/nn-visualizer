#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include <iostream>
#include "Neuron.h"

struct Pixel {
	float val;
};

struct Point {
	int x,y;
};

std::string truncate(float n);

struct Window {
	
	SDL_Renderer *renderer;
	SDL_Window *window;
	
	SDL_Surface *textSurface;
	
	int height;
	int width;
	
	TTF_Font* font;
	int font_size = 44;
	SDL_Color fgColor;
	SDL_Color bgColor;
	
	Network* net;

	bool running = false;
	bool mousedown = false;
	bool rightmousedown = false;
	
	void create_window();
	
	float value;
	
	void draw_network();
	void draw_neuron(Neuron*,int x,int y);
	void draw_circle(int,int,int);
	void draw_text(std::string str,int x,int y);
	void draw_text(float f,int x,int y) {
		this->draw_text(truncate(f),x,y);
	}
	
	void input();
	
	//take input and refresh screen
	void update(Network*);
	
	Window(int _p,int _s): width(_p), height(_s) {
		this->create_window();
		// this->populate();
		this->font = TTF_OpenFont("ubuntu.ttf", this->font_size);
		this->fgColor = { 255,255,255 };
		this->bgColor = { 0,0,0 };
		this->textSurface = TTF_RenderText_Shaded(font, std::to_string(this->value).c_str(), this->fgColor, this->bgColor);
		this->running = true;
	}
	
};