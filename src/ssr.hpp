#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>

#define PI 3.1415


namespace ssr
{
	//data
	struct vertex
	{
		float x;
		float y;
		float z;

		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	//renderer
	class renderer
	{
		private:
			int res_x=1920;
			int res_y=1080;
			SDL_Window *window = NULL; //set in constructor
			SDL_Surface *backbuffer = NULL; //set in constructor
			uint32_t pixel_type; //set in constructor

			//renderering variables
			float z_near=0.01;
			float z_far=10;
			float fov=0.5*PI; //in radians

			glm::mat4 perspective_mat; //set in constructor

			float *z_buffer = NULL; //set in constructor

		private:
			void raster_line(glm::ivec2 start, glm::ivec2 end, uint8_t r, uint8_t g, uint8_t b);
			void raster_triangle(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3);

		public:
			renderer();
			void clear(uint8_t r, uint8_t g, uint8_t b);
			void draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
			void render(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3);

	};
}