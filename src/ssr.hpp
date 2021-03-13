#pragma once

#include <glm/glm.hpp>

namespace ssr
{
	//data
	struct vertex
	{
		float x;
		float y;
		float z;

		float r;
		float g;
		float b;
	};


	int init();
	//renderer
	void render(struct ssr::vertex vertex);
}