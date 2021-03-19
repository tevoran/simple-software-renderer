#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <iostream>

#include "ssr.hpp"

int main()
{
	//initializing renderer
	ssr::renderer renderer;

	//test triangle
	ssr::vertex vertex1={0.9,0,0.1,255,255,255};
	ssr::vertex vertex2={0.2,0,1,255,255,255};
	ssr::vertex vertex3={0.9,0.9,1,255,255,255};


	renderer.render(vertex1);
	renderer.render(vertex2);
	renderer.render(vertex3);

	SDL_Delay(5000);
	return 0;
}