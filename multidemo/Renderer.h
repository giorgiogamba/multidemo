// Copyright 2025 Giorgio Gamba

#include <vector>
#include <thread>
#include "SDL3/SDL_stdinc.h"
#include <mutex>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct Pixel;

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

		void updateTexture(Uint32* pixels, const int startLine, const int endLine, const Pixel& pixel);

	private:

		void handleInputs();

		Uint32* getTextureRawData();

		void releaseTexture();

		void cleanScreen();

		void renderTexture();
		
		/** Prints information about the time requested to draw a frame and FPS */
		void printStatistics();

		int width;
		int height;

		int pitch;

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;

		std::vector<std::thread> threads;

		/** Time when the frame started to be drawn */
		std::chrono::high_resolution_clock::time_point frameStartTime;

		bool bRunning;
	};
}
