// Copyright 2025 Giorgio Gamba

#include <vector>
#include <thread>
#include "SDL3/SDL_stdinc.h"
#include <mutex>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace multidemo
{
	class Renderer
	{
	public:

		Renderer(const int inWidth, const int inHeight);

		~Renderer();

		void run();

		void update();
		void render();

		void updateTexture(Uint32* pixels, const int startingLine, const int numLines, const int width, const int value);

	private:

		int width;

		int height;

		int pitch = 0;

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;

		std::vector<std::thread> threads;

		std::mutex pixelMutex;
	};
}
