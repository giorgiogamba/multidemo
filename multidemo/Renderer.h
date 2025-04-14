// Copyright 2025 Giorgio Gamba

#pragma region Includes

#include <vector>
#include <thread>
#include <mutex>
#include <queue>

#include "SDL3/SDL_stdinc.h"

#include "Task.h"

#pragma endregion

#pragma region Forward Declarations

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct Pixel;

#pragma endregion

namespace multidemo
{
	static std::string windowTitle("multidemo");

	class Renderer
	{
	public:

		Renderer(const int inWidth, const int inHeight);
		~Renderer();

		void run();

	private:

		void update();
		void render();

		void updateTexture(const RenderTask& task);

		void handleInputs();

		void getTextureRawData();

		void releaseTexture();

		void cleanScreen();

		void renderTexture();
		
		/** Prints information about the time requested to draw a frame and FPS */
		void printStatistics() const;

		void createRenderTasks();

		void addTask(const int startLine, const int endLine, const int r, const int g, const int b);
		RenderTask getTask();

		void completeThreads();

		int width;
		int height;

		int pitch;

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;

		std::vector<std::thread> threads;
		int linesPerThread;

		/** Time when the frame started to be drawn */
		std::chrono::high_resolution_clock::time_point frameStartTime;

		bool bRunning;

		std::queue<RenderTask> tasks;

		// Permits threads to lock queue when they are looking for a task
		std::mutex tasksLock;

		std::condition_variable taskAvailable;

		Uint32* textureRawData;
	};
}
