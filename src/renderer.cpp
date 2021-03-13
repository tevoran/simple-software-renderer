#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <iostream>

void ssr::render(struct ssr::vertex vertex)
{
	//show vertex data
	std::cout << "render vertex" << std::endl;
	std::cout << "location: " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	std::cout << "color: " << vertex.r << " " << vertex.g << " " << vertex.b << std::endl;

	
	return;	
}