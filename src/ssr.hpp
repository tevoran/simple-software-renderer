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
			int res_x=800;
			int res_y=600;
			SDL_Window *window = NULL; //set in constructor
			SDL_Surface *backbuffer = NULL; //set in constructor
			uint32_t pixel_type; //set in constructor

			//renderering variables
			float z_near=0.01;
			float z_far=10;
			float fov=0.5*PI; //in radians

			glm::mat4 perspective_mat; //set in constructor

		private:


		public:
			renderer();
			void draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
			void render(struct ssr::vertex vertex);

	};
}