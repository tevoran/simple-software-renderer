#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <iostream>

#include "ssr.hpp"

int main()
{
	//initializing renderer
	try
	{
		ssr::renderer renderer;

		//test triangle
		ssr::vertex vertex1={0,0,0.0,255,255,255};
		ssr::vertex vertex2={0.2,0.1,1,255,255,255};
		ssr::vertex vertex3={0.1,0.1,1,255,255,255};


		renderer.render(vertex1, vertex2);
	}
	catch(const char *msg)
	{
		std::cout << msg << std::endl;
	}



	SDL_Delay(5000);
	return 0;
}