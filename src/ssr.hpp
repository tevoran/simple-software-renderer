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

		float u; //has to be between 0 and 1
		float v; //has to be between 0 and 1
	};

	struct pixel
	{
		int32_t x;
		int32_t y;

		uint8_t r;
		uint8_t g;
		uint8_t b;

		uint64_t z;

		float u; //has to be between 0 and 1
		float v; //has to be between 0 and 1
	};

	class texture
	{
	public:
		//dimensions of the texture
		uint32_t x;
		uint32_t y;

		unsigned char *pixel_data;
	public:
		texture(const char *path);
	};


	//renderer
	class renderer
	{
		private:
			int res_x;
			int res_y;
			SDL_Window *window = NULL; //set in constructor
			SDL_Surface *backbuffer = NULL; //set in constructor
			uint32_t pixel_type; //set in constructor

			//renderering variables
			float z_near=0.01;
			float z_far=10;
			float fov=0.5*PI; //in radians

			glm::mat4 perspective_mat; //set in constructor

			uint64_t *z_buffer = NULL; //set in constructor

		private:
			void raster_line(glm::ivec2 start, glm::ivec2 end, uint8_t r, uint8_t g, uint8_t b);
			void raster_triangle(struct ssr::vertex *vertex1, struct ssr::vertex *vertex2, struct ssr::vertex *vertex3, const ssr::texture *texture, uint32_t flags);

			void vertex_shader(ssr::vertex *vertex, glm::vec3 *mesh_pos, glm::vec3 *rot_axis, float rot_angle);

				class triangle_line_rendering
				{
					private:
						//end points of the line
						glm::ivec2 start;
						glm::ivec2 end;

						glm::ivec2 d;

						int32_t t,t_end, t_step, dt; //slow axis
						int32_t s,s_end, s_step, ds; //fast axis

						int32_t dt_cond_add; //negative for positve dt;
						int32_t ds_cond_add; //positive for positve ds;

						bool fast_x;

						int32_t error;


					public:
						triangle_line_rendering(glm::ivec2 in_start, glm::ivec2 in_end);
						int32_t triangle_line_iterate(); //returns current x-value of the line
						void show_variables();
				};


		public:
			renderer(int res_x_in, int res_y_in, float fov_y, float aspect_ratio, float near_z_clip, float far_z_clip);
			~renderer();
			void update();
			void draw_pixel(struct ssr::pixel *data); //make sure that your pixel is on screen otherwise there is undefined behaviour
			void draw_pixel_fast(struct ssr::pixel *data, uint32_t pixel_offset); //draws at the pixel offset and the z-buffer check needs already to be done
			void texture_map(struct ssr::pixel *data, const struct ssr::texture *texture);
			void render(struct vertex *data, uint32_t num_polygons, glm::vec3 mesh_pos, glm::vec3 *rot_axis, float rot_angle, const ssr::texture *texture, uint32_t flags);
			void validate_window();
			ssr::vertex* load_mesh(const char * path, uint32_t *num_polygons, glm::vec3 *max_coords, glm::vec3 *min_coords);

	};
}