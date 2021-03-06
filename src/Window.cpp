#include "Window.h"

#include "Utility.h"
#include <SDL2/SDL.h>
#include <iostream>

Window::Window(Network *network): net(network)  {
	this->width = 900;
	this->topology = this->net->topology;
	if(this->topology.size() > 4) {
		this->width += (220*(this->topology.size()-4));
	}
	this->height = 1000;
	this->create_window();
	// this->populate();
	this->font = TTF_OpenFont("ubuntu.ttf", this->font_size);
	this->fgColor = { 255,255,255 };
	this->bgColor = { 0,0,0 };
	this->textSurface = TTF_RenderText_Shaded(font, std::to_string(this->trials).c_str(), this->fgColor, this->bgColor);
	this->running = true;
}

std::string truncate(double n) {
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
					case (int)'a':
						this->net->alpha -= .01;
						_alpha -= .01;
						break;
					case (int)'s':
						this->net->alpha += .01;
						_alpha += .01;
						break;
					case (int)'e':
						this->net->eta += .01;
						// _eta += .01;
						break;
					case (int)'w':
						this->net->eta -= .01;
						// _eta -= .01;
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

void Window::draw_circle(int n_cx, int n_cy, int radius,Uint8 r,Uint8 g,Uint8 b,Uint8 a) {
	
	// n_cx-=radius/2;
	// n_cy -= radius/2;

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

void Window::draw_neuron(Neuron *neuron,int x,int y,int r) {
	this->draw_text(this->show_gradient? neuron->gradient : neuron->value,x-35,y-20);
	// SDL_SetRenderDrawColor(this->renderer,255,255,255,255);
	// SDL_RenderDrawPoint(this->renderer, x,y);
	double value = (neuron->value);
	Uint8 red,green,blue;
	
	if(value > 1) {
		value=1;
	}
	if(value < 0) {
		value = fabs(value);
		red = 255*(1-value);
		green = 0;
		blue = 255*(value);
	} else {
		red = 255*(1-value);
		green = 255*value;
		blue = 0;
	}
	if(neuron->bias) {
		this->draw_circle(x,y,r,50,50,255,255);
	} else {
		this->draw_circle(x,y,r,red,green,blue,255);
	}
}

void Window::draw_network() {
	
	int startx = 90;
	int starty = 200;
	int x = startx;
	int y = starty;
	
	int xspace = 120;
	int yspace = 20;
	
	float scale = 1;
	
	float neuron_radius = 50.0f*scale;
	

	xspace *= scale;
	yspace *= scale;	
	
	this->draw_text("Error: "+truncate(this->net->err),250,20);
	this->draw_text("Trials: "+std::to_string(this->trials),250,80);
	this->draw_text("Eta: "+truncate(this->net->eta),20,20);
	this->draw_text("Alpha: "+truncate(this->net->alpha),20,80);
	std::string display = this->show_gradient? "Gradient" : "Value";
	this->draw_text("Tickrate: "+std::to_string(this->tickrate),500,20);
	this->draw_text("Display: "+display,500,80);
	this->draw_text("Ex: "+this->stage,750,20);
	
	for(int i=0;i<this->net->layers.size();i++) {
		Layer *layer = this->net->layers.at(i);
		for(auto &neuron : layer->neurons) {
			this->draw_neuron(neuron,x,y,neuron_radius);
			if(i == this->net->layers.size()-1) {
				y += (neuron_radius*2)+yspace;
				continue;
			}
			Layer *next = this->net->layers.at(i+1);
			int tx = x;
			int ty = starty;
			for(int j=0;j<next->neurons.size();j++) {
				if(next->neurons.at(j)->bias) continue;
				double weight = (neuron->weights.at(next->neurons.at(j)));
				int r,g,b;
				if(weight > 1) {
					weight=1;
				}
				if(weight < 0) {
					weight = fabs(weight);
					r = 255*(1-weight);
					g = 0;
					b = 255*(weight);
				} else {
					r = 255*(1-weight);
					g = 255*weight;
					b = 0;
				}
				
				SDL_SetRenderDrawColor(this->renderer,r,g,b,255);
				SDL_RenderDrawLine(this->renderer,x+neuron_radius,y,tx+(xspace)+(neuron_radius),ty);
				ty += yspace+(neuron_radius*2);
			}
			// ty += yspace;
			SDL_SetRenderDrawColor(this->renderer,0,0,0,255);
			y += (neuron_radius*2)+yspace;
		}
		y = starty;
		x += (neuron_radius*2)+xspace;
	}
}

void Window::update() {
	this->input();
	SDL_RenderClear(this->renderer);

	this->draw_network();
	
	SDL_RenderPresent(this->renderer);
	SDL_Delay(1000/this->tickrate);
}