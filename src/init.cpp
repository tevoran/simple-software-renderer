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

	return 0;
}