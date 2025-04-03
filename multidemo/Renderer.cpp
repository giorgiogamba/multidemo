// Copyright 2025 Giorgio Gamba

#include "Renderer.h"

#include <iostream>

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"

namespace multidemo
{

	Renderer::Renderer(const int inWidth, const int inHeight)
		: width(inWidth)
		, height(inHeight)
	{
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow("TEST", width, height, 0);
		if (!window)
		{
			return;
		}

		SDL_SetWindowResizable(window, true);

		renderer = SDL_CreateRenderer(window, nullptr);
		if (!renderer)
		{
			return;
		}
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

	void Renderer::run()
	{
		while (true)
		{
			render();
		}
	}

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
