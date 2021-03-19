#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>

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
			SDL_Surface *backbuffer = NULL;
			uint32_t pixel_type; //set in constructor

		private:

		public:
			void draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
			renderer();
			void render(struct ssr::vertex vertex);

	};
}