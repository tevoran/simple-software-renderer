#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <math.h>

ssr::renderer::renderer()
{
	//SDL INIT
	if(SDL_Init(SDL_INIT_EVERYTHING)==0)
		{
			std::cout << "SDL successfully initialized" << std::endl;
		}
		else
		{
			std::cout << "SDL ERROR while initializing SDL" << std::endl;
		}

	//SDL creating window
	window = SDL_CreateWindow(	"Simple Software Renderer - by Johannes Pfeiffer",
											SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											res_x, 
											res_y, 
											0);
		if(window==NULL)
		{
			std::cout << "SDL couldn't create window" << std::endl;
		}

	//creating render surfaces
	backbuffer = SDL_GetWindowSurface(window);
		if(backbuffer==NULL)
		{
			std::cout << "SDL couldn't create render surface" << std::endl;
		}
		else
		{
			//getting pixel format and setting pixel format variable accordingly
			pixel_type = backbuffer->format->format;
			std::cout << "Format: " << SDL_GetPixelFormatName(pixel_type) << std::endl;
		}

	//renderer initialization
	perspective_mat[0].x=tan(0.5*PI-0.5*fov);
	perspective_mat[0].y=0;
	perspective_mat[0].z=0;
	perspective_mat[0].w=0;

	perspective_mat[1].x=0;
	perspective_mat[1].y=1;
	perspective_mat[1].z=0;
	perspective_mat[1].w=0;

	perspective_mat[2].x=0;
	perspective_mat[2].y=0;
	perspective_mat[2].z=1;
	perspective_mat[2].w=0;

	perspective_mat[3].x=0;
	perspective_mat[3].y=0;
	perspective_mat[3].z=0;
	perspective_mat[3].w=1;

}

void ssr::renderer::draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	//RGB pixel format
	//highest byte is red, lowest byte is blue
	if(pixel_type == SDL_PIXELFORMAT_RGB888)
	{
		uint32_t *pixel_ptr = static_cast<uint32_t*>(backbuffer->pixels);
		uint32_t pixel_colored = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGB888), r, g, b);
		pixel_ptr[x+y*(backbuffer->w)]=pixel_colored;

	}
	else
	{
		std::cout << "SDL: unknown pixel format" << std::endl;
	}
}

/*the renderer uses a clip space that is similar to OpenGL. But the clip space's borders are
0 and 1 along the different axes.*/
void ssr::renderer::render(struct ssr::vertex vertex)
{
	//counting the rendered vertices
	static unsigned int num_vertices = 0;
	num_vertices++;

	//show vertex data
	std::cout << "render vertex " << num_vertices << std::endl;
	std::cout << "location: " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	std::cout << "color: " << (int)vertex.r << " " << (int)vertex.g << " " << (int)vertex.b << std::endl;

	//vertex transformation aka vertex shader
	glm::vec4 vex={vertex.x, vertex.y, vertex.z, 0};


	//rasterization
	int32_t x,y;
	x=(float)vex.x*(backbuffer->w);
	y=(float)vex.y*(backbuffer->h);


	std::cout << "XxY: " << vex.x << " " << vex.y << std::endl;
	std::cout << "XxY (rasterized): " << x << "x" << y << std::endl;

	draw_pixel(x,y,vertex.r, vertex.g, vertex.b);

	//render changed backbuffer onto the screen
	if(SDL_UpdateWindowSurface(window)!=0)
	{
		std::cout << "SDL ERROR MSG: " << std::endl << SDL_GetError() << std::endl;
	}
	return;	
}