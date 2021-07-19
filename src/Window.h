#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include <iostream>

struct Pixel {
	float val;
};

struct Point {
	int x,y;
};

struct Window {
	
	SDL_Renderer *renderer;
	SDL_Window *window;
	
	SDL_Surface *textSurface;
	
	int height;
	int width;
	
	TTF_Font* font;
	SDL_Color fgColor;
	SDL_Color bgColor;

	bool running = false;
	bool mousedown = false;
	bool rightmousedown = false;
	
	void create_window();
	
	float value;
	
	void draw_pixels();
	
	void input();
	
	//take input and refresh screen
	void update();
	
	Window(int _p,int _s): width(_p), height(_s) {
		this->create_window();
		// this->populate();
		this->font = TTF_OpenFont("ubuntu.ttf", 64);
		this->fgColor = { 255,255,255 };
		this->bgColor = { 0,0,0 };
		this->textSurface = TTF_RenderText_Shaded(font, std::to_string(this->value).c_str(), this->fgColor, this->bgColor);
		this->running = true;
	}
	
};