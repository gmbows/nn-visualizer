#include "Window.h"

#include <SDL2/SDL.h>
#include <iostream>

std::string truncate(float n) {
	std::string s = std::to_string(n);
	return s.substr(0,std::min((int)s.size(),4));
}

void Window::create_window() {
	this->window = SDL_CreateWindow("NN Visualizer",
												SDL_WINDOWPOS_UNDEFINED,
												SDL_WINDOWPOS_UNDEFINED,
												this->width,this->height,
												SDL_WINDOW_OPENGL
												);

	this->renderer = SDL_CreateRenderer(this->window,-1,SDL_RENDERER_ACCELERATED);
}

void Window::input() {
	SDL_Event event;
	int keycode;
	while(SDL_PollEvent(&event)) {
	
	keycode = event.key.keysym.sym; 

		switch(event.type) {
			case SDL_QUIT:
				this->running = false;
				break;
			case SDL_KEYDOWN:
				switch(keycode) {
					case 27:
					case (int)'q':
						this->running = false;
						break;
					case (int)' ':
						this->pause = !this->pause;
						break;
					case (int)'g':
						this->show_gradient = !this->show_gradient;
						break;
					case SDLK_DOWN:
						this->tickrate--;
						break;
					case SDLK_UP:
						this->tickrate++;
						break;
				}
			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)  {
					case SDL_BUTTON_LEFT:
						this->mousedown = true;
						break;
					case SDL_BUTTON_RIGHT:
						this->rightmousedown = true;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch(event.button.button)  {
					case SDL_BUTTON_LEFT:
						this->mousedown = false;
						break;
					case SDL_BUTTON_RIGHT:
						this->rightmousedown = false;
						break;
				}
				break;
		}
	}
}

void Window::draw_circle(int n_cx, int n_cy, int radius) {
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:

	Uint8 a = 255;
	Uint8 r = 255;
	Uint8 g = 255;
	Uint8 b = 255;

	double error = (double)-radius;
	double x = (double)radius - 0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;
	
	SDL_SetRenderDrawColor(this->renderer,r,g,b,a);

	while (x >= y)
	{
		SDL_RenderDrawPoint(this->renderer, (int)(cx + x), (int)(cy + y));
		SDL_RenderDrawPoint(this->renderer, (int)(cx + y), (int)(cy + x));

		if (x != 0)
		{
			SDL_RenderDrawPoint(this->renderer, (int)(cx - x), (int)(cy + y));
			SDL_RenderDrawPoint(this->renderer, (int)(cx + y), (int)(cy - x));
		}

		if (y != 0)
		{
			SDL_RenderDrawPoint(this->renderer, (int)(cx + x), (int)(cy - y));
			SDL_RenderDrawPoint(this->renderer, (int)(cx - y), (int)(cy + x));
		}

		if (x != 0 && y != 0)
		{
			SDL_RenderDrawPoint(this->renderer, (int)(cx - x), (int)(cy - y));
			SDL_RenderDrawPoint(this->renderer, (int)(cx - y), (int)(cy - x));
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}
	SDL_SetRenderDrawColor(this->renderer,0,0,0,255);
}

void Window::draw_text(std::string str,int x,int y) {
	SDL_Rect textLocation = { x, y, 18*(int)str.size(),40};
	this->textSurface = TTF_RenderText_Shaded(font, str.c_str(), this->fgColor, this->bgColor);
	SDL_Texture *text = SDL_CreateTextureFromSurface(this->renderer,this->textSurface);
	SDL_RenderCopy(this->renderer,text,NULL,&textLocation);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(text);
}

void Window::draw_neuron(Neuron *neuron,int x,int y) {
	this->draw_text(this->show_gradient? neuron->gradient : neuron->value,x-13,y);
	this->draw_circle(x+20,y+20,50);
}

void Window::draw_network() {
	
	int startx = 100;
	int starty = 200;
	int x = startx;
	int y = starty;
	
	int xspace = 120;
	int yspace = 20;
	
	
	this->draw_text("Error: "+truncate(this->net->err),250,20);
	this->draw_text("Trials: "+std::to_string(this->trials),250,80);
	this->draw_text("Eta: "+truncate(this->net->eta),20,20);
	this->draw_text("Alpha: "+truncate(this->net->alpha),20,80);
	std::string display = this->show_gradient? "Gradient" : "Value";
	this->draw_text("Tickrate: "+std::to_string(this->tickrate),500,20);
	this->draw_text("Display: "+display,500,80);
	// this->draw_text("Stage: "+this->stage,730,20);
	
	
	for(int i=0;i<this->net->layers.size();i++) {
		Layer *layer = this->net->layers.at(i);
		for(auto &neuron : layer->neurons) {
			this->draw_neuron(neuron,x,y);
			if(i == this->net->layers.size()-1) return;
			Layer *next = this->net->layers.at(i+1);
			int tx = x;
			int ty = starty;
			for(int j=0;j<next->neurons.size();j++) {
				float weight = fabs(neuron->weights.at(next->neurons.at(j)));
				SDL_SetRenderDrawColor(this->renderer,255*(1-weight),255*weight,0,255);
				SDL_RenderDrawLine(this->renderer,x+50+yspace,y+yspace,tx+(xspace*2)-50,ty+yspace);
				ty+= yspace+100;
			}
			// ty += yspace;
			SDL_SetRenderDrawColor(this->renderer,0,0,0,255);
			y += 100+yspace;
		}
		y = starty;
		x += 100+xspace;
	}
}

void Window::update(Network* network) {
	this->net = network;
	this->input();
	SDL_RenderClear(this->renderer);

	this->draw_network();
	
	SDL_RenderPresent(this->renderer);
	SDL_Delay(1000/this->tickrate);
}