#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <iostream>

#include "ssr.hpp"

int main()
{
	//initializing renderer
	ssr::renderer renderer;

	//test triangle
	ssr::vertex vertex1={0,0,1,1,1,1};
	ssr::vertex vertex2={1,0,1,1,1,1};
	ssr::vertex vertex3={0,1,1,1,1,1};

	renderer.draw_pixel(100,100,255,255,255);

	int y=100;
	for(int x=0; x<600; x++)
	{
		renderer.draw_pixel(x,y,255,255,255);
	}

	renderer.render(vertex1);
	renderer.render(vertex2);
	renderer.render(vertex3);

	SDL_Delay(5000);
	return 0;
}