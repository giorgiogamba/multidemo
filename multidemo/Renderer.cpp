// Copyright 2025 Giorgio Gamba

#include "Renderer.h"

#include <iostream>
#include <atomic>

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

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
		if (!texture)
		{
			return;
		}

		const int numThread = std::thread::hardware_concurrency() - 1;
		threads.resize(numThread);
	}

	Renderer::~Renderer()
	{
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

	void Renderer::run()
	{
		while (true)
		{
			update();
			render();
		}
	}

	void Renderer::update()
	{
		void* rawPixels = nullptr;
		

		// Locks texture in order to obtaint access to pixels/
		// The pitch is the number of bytes per pixel
		if (!SDL_LockTexture(texture, nullptr, &rawPixels, &pitch)) {
			return;
		}

		Uint32* pixels = static_cast<Uint32*>(rawPixels); // Cast to 32bit type because it assumes ARGB888 format

		const int numLines = height / (std::thread::hardware_concurrency() - 1);

		for (int threadID = 0; threadID < threads.size()-1; ++threadID)
		{
			const int startingLine = threadID * numLines;
			const int value = 20 * threadID;
			const int inWidth = width;
			threads[threadID] = std::thread(&Renderer::updateTexture, this, pixels, startingLine, numLines, width, value);
		}

		for (int threadID = 0; threadID < threads.size() - 1; ++threadID)
		{
			if (threads[threadID].joinable())
			{
				threads[threadID].join();
			}
		}

		// Unlocks texture after writing
		SDL_UnlockTexture(texture);
	}

	void Renderer::render()
	{
		SDL_SetRenderScale(renderer, 4, 4);

		// Clean up
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw texture over the window completely (otherwise draw a rectangle)
		SDL_RenderTexture(renderer, texture, nullptr, nullptr);

		// Show result
		SDL_RenderPresent(renderer);
	}

	void Renderer::updateTexture(Uint32* pixels, const int startingLine, const int numLines, const int width, const int value)
	{
		for (int y = startingLine; y < startingLine + numLines; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const int idx = y /** pitch*/ + x * sizeof(Uint32);
				Uint32* targetPixel = (Uint32*)&pixels[idx];

				const Uint8 red = static_cast<Uint8>(value);
				const Uint8 green = static_cast<Uint8>(value);
				const Uint8 blue = static_cast<Uint8>(value);
				const Uint8 alpha = static_cast<Uint8>(255);

				// Composes the pixels in format ARGB
				if (targetPixel)
				{
					*targetPixel = (alpha << 24) | (red << 16) | (green << 8) | blue;
				}
			}
		}
	}
}
