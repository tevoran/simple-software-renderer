#include <glm/glm.hpp>
#include <iostream>

#include "ssr.hpp"

int main()
{
	ssr::init();
	//test triangle
	ssr::vertex vertex1={0,0,1,1,1,1};
	ssr::vertex vertex2={1,0,1,1,1,1};
	ssr::vertex vertex3={0,1,1,1,1,1};

	ssr::render(vertex1);
	ssr::render(vertex2);
	ssr::render(vertex3);

	return 0;
}