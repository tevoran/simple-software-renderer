#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>

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

	SDL_Delay(1000);
}

void ssr::renderer::draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	//RGB pixel format
	//highest byte is red, lowest byte is blue
	if(pixel_type == SDL_PIXELFORMAT_RGB888)
	{
		std::cout << "drawing_pixel: " << x << "x" << y << " in color " << (int)r << "x" << (int)g << "x" << (int)b << std::endl;

		uint8_t *pixel_ptr = static_cast<uint8_t*>(backbuffer->pixels);
		uint32_t pixel_colored = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGB888), r, g, b);
		std::cout << std::hex << pixel_colored << std::endl;
		pixel_ptr[x+y*(backbuffer->w)]=r;
		pixel_ptr[x+y*(backbuffer->w)+1]=g;
		pixel_ptr[x+y*(backbuffer->w)+2]=b;

	}
	else
	{
		std::cout << "SDL: unknown pixel format" << std::endl;
	}
}

void ssr::renderer::render(struct ssr::vertex vertex)
{
	//counting the rendered vertices
	static unsigned int num_vertices = 0;
	num_vertices++;

	//show vertex data
	std::cout << "render vertex " << num_vertices << std::endl;
	std::cout << "location: " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	std::cout << "color: " << vertex.r << " " << vertex.g << " " << vertex.b << std::endl;

	//render changed backbuffer onto the screen
	if(SDL_UpdateWindowSurface(window)!=0)
	{
		std::cout << "SDL ERROR MSG: " << std::endl << SDL_GetError() << std::endl;
	}
	return;	
}