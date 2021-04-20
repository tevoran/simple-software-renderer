#include "ssr.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>


void ssr::renderer::vertex_shader(ssr::vertex *vertex)
{
	glm::vec4 tmp=glm::vec4(vertex->x, vertex->y, vertex->z, 1.0);
	glm::vec4 out=perspective_mat * tmp;
	
//perspective divide and offset for screen, because of engine intern clipspace
	vertex->x=out.x/vertex->z+0.5;
	vertex->y=out.y/vertex->z+0.5;
	vertex->z=out.z;
}