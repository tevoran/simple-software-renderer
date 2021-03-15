#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

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
	SDL_Window *window = SDL_CreateWindow(	"Simple Software Renderer - by Johannes Pfeiffer",
											SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											res_x, 
											res_y, 
											0);
		if(window==NULL)
		{
			std::cout << "SDL couldn't create window" << std::endl;
		}

	//creating render surface
	surface = SDL_CreateRGBSurfaceWithFormat(	0, 
												res_x,
												res_y,
												32,
												SDL_PIXELFORMAT_RGBA32);
		if(surface==NULL)
		{
			std::cout << "SDL couldn't create render surface" << std::endl;
		}

	SDL_Delay(1000);
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

	
	return;	
}