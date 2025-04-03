// Copyright 2025 Giorgio Gamba

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

		void render();

	private:

		int width;

		int height;

		SDL_Window* window;
		SDL_Renderer* renderer;
	};
}
