#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>


namespace ssr
{
	//data
	struct vertex
	{
		float x;
		float y;
		float z;

		float r;
		float g;
		float b;
	};

	//renderer
	class renderer
	{
		private:
			int res_x=800;
			int res_y=600;
			SDL_Window *window = NULL;
			SDL_Surface *surface = NULL;

		public:
			renderer();

	};
	void render(struct ssr::vertex vertex);
}