#include "Window.h"

#include <SDL2/SDL.h>
#include <iostream>

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


void Window::draw_pixels() {
	SDL_Rect textLocation = { 500, 400, 200, 50};
	this->textSurface = TTF_RenderText_Shaded(font, std::to_string(this->value).c_str(), this->fgColor, this->bgColor);
	SDL_Texture *text = SDL_CreateTextureFromSurface(this->renderer,this->textSurface);
	SDL_RenderCopy(this->renderer,text,NULL,&textLocation);
}

void Window::update() {
	this->input();
	SDL_RenderClear(this->renderer);

	this->draw_pixels();
	
	SDL_RenderPresent(this->renderer);
	SDL_Delay(1000/144);
}