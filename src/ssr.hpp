#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>

#define PI 3.1415
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
			void raster_triangle(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3, uint32_t flags);
				//uint32_t raster_triangle_line_step(glm::ivec2 start, glm::ivec2 end, uint32_t flags); //the function returns true, if the y-value increases

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


						//returns
						bool y_update_ready=false;
						bool line_finished=false;
					public:
						triangle_line_rendering(glm::ivec2 in_start, glm::ivec2 in_end);
						void triangle_line_iterate();
						void show_variables();
						glm::ivec2 get_location();

						bool line_done();
						bool y_update();
						void y_update_processed();
				};

		public:
			renderer();
			void update();
			void draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
			void render(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3, uint32_t flags);

	};
}