#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <exception>

ssr::renderer::renderer()
{
	//SDL INIT
	if(SDL_Init(SDL_INIT_EVERYTHING)==0)
		{
			std::cout << "SDL successfully initialized" << std::endl;
		}
		else
		{
			throw "SDL ERROR while initializing SDL";
		}

	//SDL creating window
	window = SDL_CreateWindow(	"Simple Software Renderer - by Johannes Pfeiffer",
											SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											res_x, 
											res_y, 
											0);
		if(window==NULL)
		{
			throw "SDL couldn't create window";
		}

	//creating render surfaces
	backbuffer = SDL_GetWindowSurface(window);
		if(backbuffer==NULL)
		{
			throw "SDL couldn't create render surface";
		}
		else
		{
			//getting pixel format and setting pixel format variable accordingly
			pixel_type = backbuffer->format->format;
			std::cout << "Format: " << SDL_GetPixelFormatName(pixel_type) << std::endl;
		}

	//renderer initialization
		//Z-Buffer initialization
		z_buffer=new float[backbuffer->w * backbuffer->h];

		//Projection matrix
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

//clears the background to black
void ssr::renderer::update()
{
	//render changed backbuffer onto the screen
	if(SDL_UpdateWindowSurface(window)!=0)
	{
		std::cout << "SDL ERROR MSG: " << std::endl << SDL_GetError() << std::endl;
	}

	//RGB pixel format
	//highest byte is red, lowest byte is blue
	if(pixel_type == SDL_PIXELFORMAT_RGB888)
	{
		void *pixel_ptr = static_cast<void*>(backbuffer->pixels);
		memset(pixel_ptr, 0, (backbuffer->h)*(backbuffer->w)*sizeof(uint32_t));
	}
	else
	{
		throw "SDL: unknown pixel format and cannot clear the screen";
	}

}

void ssr::renderer::draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	if(x<(backbuffer->w) && x>=0 && y>=0 && y<(backbuffer->h))
	{
		//RGB pixel format
		//highest byte is red, lowest byte is blue
		if(pixel_type == SDL_PIXELFORMAT_RGB888)
		{
				static uint32_t *pixel_ptr = static_cast<uint32_t*>(backbuffer->pixels);
				uint32_t pixel_colored = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGB888), r, g, b);
				pixel_ptr[x+y*(backbuffer->w)]=pixel_colored;
		}
		else
		{
			throw "SDL: unknown pixel format and cannot draw a pixel";
		}
	}
}

/*the renderer uses a clip space that is similar to OpenGL. But the clip space's borders are
0 and 1 along the different axes.*/
void ssr::renderer::render(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3, uint32_t flags)
{
	//counting the rendered vertices
	static unsigned int num_vertices = 0;
	

	raster_triangle(vertex1, vertex2, vertex3, flags);





	//vertex transformation aka vertex shader
	//glm::vec4 vex={vertex1.x, vertex1.y, vertex1.z, 0};

	return;	
}