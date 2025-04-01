// Copyright 2025 Giorgio Gamba

#include "SDL3/SDL_init.h"

class Renderer
{
public:

	Renderer(const int inWidth, const int inHeight)
		: width(inWidth)
		, height(inHeight)
	{
		SDL_Init(SDL_INIT_VIDEO);
	
		window = SDL_CreateWindow("TEST", width, height, SDL_WINDOWPOS_CENTERED | SDL_WINDOWPOS_CENTERED);

	};

private:

	int width, height;

	SDL_Window* window;
};