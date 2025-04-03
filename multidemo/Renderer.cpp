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
	void Renderer::render()
	{
		SDL_SetRenderScale(renderer, 4, 4);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				SDL_RenderPoint(renderer, i, j);
			}
		}

		SDL_RenderPresent(renderer);
	}
}
