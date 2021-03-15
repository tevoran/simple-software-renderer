#include <glm/glm.hpp>
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

	renderer.render(vertex1);
	renderer.render(vertex2);
	renderer.render(vertex3);

	return 0;
}