#pragma once

#include <glm/glm.hpp>

namespace ssr
{
	struct vertex
	{
		float x;
		float y;
		float z;

		float r;
		float g;
		float b;
	};
}

float* ssr_get_data();