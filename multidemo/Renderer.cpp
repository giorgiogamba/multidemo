// Copyright 2025 Giorgio Gamba

#include "Renderer.h"

#include <iostream>
#include <atomic>
#include <chrono>

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "Pixel.h"

namespace multidemo
{

	Renderer::Renderer(const int inWidth, const int inHeight)
		: width(inWidth)
		, height(inHeight)
		, pitch(0)
		, window(nullptr)
		, renderer(nullptr)
		, texture(nullptr)
		, bRunning(true)
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

		// We don't make the main thread draw
		const int numThreads = std::thread::hardware_concurrency() - 1;
		threads.resize(numThreads);

		std::cout << "Renderer spawned with " << numThreads + 1 << " threads\n";
	}

	Renderer::~Renderer()
	{
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

	void Renderer::run()
	{
		bool bRunning = true;
		while (bRunning)
		{
			// start timer
			frameStartTime = std::chrono::high_resolution_clock::now();
			
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT) {
					bRunning = false;
					break;
				}
			}

			update();
			render();
			printStatistics();
		}
	}

	void Renderer::update()
	{
		Uint32* pixels = getTextureRawData();
		if (!pixels)
			return;

		int startingLine = 0;
		const int linesPerThread = height / static_cast<int>(threads.size() + 1);

		constexpr int threadColorValue = 10;
		for (int i = 0; i < threads.size(); ++i)
		{
			const int endLine = startingLine + linesPerThread;
			
			const int currentThreadColorValue = i * threadColorValue;
			Pixel pixel(currentThreadColorValue, currentThreadColorValue, currentThreadColorValue);
			threads[i] = std::thread(&Renderer::updateTexture, this, pixels, startingLine, endLine, pixel);

			startingLine = endLine;
		}

		// make the main thread draw all the remaining lines (considers also reminders from previous threads)
		Pixel mainThreadPixel(threadColorValue, threadColorValue, threadColorValue);
		updateTexture(pixels, startingLine, height, mainThreadPixel);

		// makes the main thread wait for each worker
		for (int i = 0; i < threads.size(); ++i)
		{
			if (threads[i].joinable())
			{
				threads[i].join();
			}
		}

		releaseTexture();
	}

	void Renderer::render()
	{
		cleanScreen();
		renderTexture();
	}

	void Renderer::updateTexture(Uint32* pixels, const int startLine, const int endLine, const Pixel& pixel)
	{
		if (!pixels)
			return;

		for (int y = startLine; y < endLine; ++y)
		{
			Uint32* row = (Uint32*)((Uint8*)pixels + y * pitch);

			for (int x = 0; x < width; ++x)
			{	
				row[x] = pixel.getColor();
			}
		}
	}

	Uint32* Renderer::getTextureRawData()
	{
		// Use pointer to void because we don't know the memory typology
		void* rawPixels = nullptr;

		// Locks texture in order to obtain access to pixels/
		// The pitch is the number of bytes per pixel
		if (!SDL_LockTexture(texture, nullptr, &rawPixels, &pitch))
		{
			return nullptr;
		}

		return static_cast<Uint32*>(rawPixels); // Cast to 32bit type because it assumes ARGB888 format
	}

	void Renderer::releaseTexture()
	{
		// Unlocks texture after writing
		SDL_UnlockTexture(texture);
	}

	void Renderer::cleanScreen()
	{
		SDL_SetRenderScale(renderer, 4, 4);

		// Clean up
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}

	void Renderer::renderTexture()
	{
		// Draw texture over the window completely (otherwise draw a rectangle)
		SDL_RenderTexture(renderer, texture, nullptr, nullptr);

		// Show result
		SDL_RenderPresent(renderer);
	}

	void Renderer::printStatistics()
	{
		const std::chrono::high_resolution_clock::time_point frameEndTime = std::chrono::high_resolution_clock::now();

		// Compute the time requested to draw the current frame
		const std::chrono::duration<double, std::milli> frameTime = frameEndTime - frameStartTime;

		// Compute fps
		constexpr std::chrono::seconds oneSecond(1);
		constexpr auto secondInMilli = std::chrono::duration_cast<std::chrono::milliseconds>(oneSecond);

		const double framesPerSecond = secondInMilli.count() / frameTime.count();

		std::cout << "Frame drawn in " << frameTime.count() << " milliseconds. FPS: " << framesPerSecond << "\n";
	}
}
