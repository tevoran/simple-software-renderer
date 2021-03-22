#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

#include "ssr.hpp"

int main()
{
	//initializing renderer
	try
	{
		ssr::renderer renderer;

		//test triangle
		ssr::vertex vertex1={0.5,0.5,1,255,255,255};
		ssr::vertex vertex2={0.5,0.4,1,255,255,255};
		ssr::vertex vertex3={0.1,0.1,1,255,255,255};


		for(float i=0; i<100; i=i+0.01)
		{
					vertex2={0.2*cos(i)+0.5, 0.2*(float)sin(i)+0.5, 1, 255, 255, 255};
					renderer.render(vertex1, vertex2, vertex3);
					renderer.clear(0,0,0);
		}

	}
	catch(const char *msg)
	{
		std::cout << msg << std::endl;
	}



	SDL_Delay(5000);
	return 0;
}