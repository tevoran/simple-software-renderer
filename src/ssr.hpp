#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>

#define PI 3.1415

#define SSR_Z_BUFFER_RES 0x000008FF
#define SSR_Z_BUFFER_CLEAR -1

#define SSR_WIREFRAME 0xFFFFFFFE
#define SSR_FILL 0xFFFFFFFD


#define SSR_TRIANGLE_Y_UPDATE 1
#define SSR_TRIANGLE_NO_Y_UPDATE 0
#define SSR_TRIANGLE_INIT 2

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

	struct pixel
	{
		int32_t x;
		int32_t y;

		uint8_t r;
		uint8_t g;
		uint8_t b;

		int64_t z;
	};

	//renderer
	class renderer
	{
		private:
			int res_x=800;
			int res_y=800;
			SDL_Window *window = NULL; //set in constructor
			SDL_Surface *backbuffer = NULL; //set in constructor
			uint32_t pixel_type; //set in constructor

			//renderering variables
			float z_near=0.01;
			float z_far=10;
			float fov=0.5*PI; //in radians

			glm::mat4 perspective_mat; //set in constructor

			int64_t *z_buffer = NULL; //set in constructor

		private:
			void raster_line(glm::ivec2 start, glm::ivec2 end, uint8_t r, uint8_t g, uint8_t b);
			void raster_triangle(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3, uint32_t flags);

				class triangle_line_rendering
				{
					private:
						//end points of the line
						glm::ivec2 start;
						glm::ivec2 end;

						glm::ivec2 d;

						int32_t t,t_end, t_step, dt; //slow axis
						int32_t s,s_end, s_step, ds; //fast axis

						bool fast_x;

						int32_t error;


					public:
						triangle_line_rendering(glm::ivec2 in_start, glm::ivec2 in_end);
						void triangle_line_iterate();
						void show_variables();
						uint32_t get_x();
				};

			int64_t get_z(float a, float b, float d, uint32_t x, uint32_t y) //coordinate form of plain has to be already divided by c
				{
					return -a*x-b*y-d;
				};

		public:
			renderer();
			~renderer();
			void update();
			void draw_pixel(struct ssr::pixel *data);
			//void draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, int32_t z);
			void render(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3, uint32_t flags);

	};
}