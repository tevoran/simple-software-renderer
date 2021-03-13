#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <iostream>

#include "ssr.hpp"

int ssr::init()
{
	//SDL INIT
	if(SDL_Init(SDL_INIT_EVERYTHING)==0)
		{
			std::cout << "SDL successfully initialized" << std::endl;
		}
		else
		{
			std::cout << "SDL ERROR while initializing SDL" << std::endl;
			return 1;
		}

	SDL_Window *window = SDL_CreateWindow(	"Simple Software Renderer - by Johannes Pfeiffer",
											SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											800, 
											600, 
											0);
		if(window==NULL)
		{
			std::cout << "SDL couldn't create window" << std::endl;
			return 1;
		}
	SDL_Delay(1000);

	return 0;
}