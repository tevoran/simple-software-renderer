#include "ssr.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include <iostream>


void ssr::renderer::vertex_shader(ssr::vertex *vertex, glm::vec3 *mesh_pos, glm::vec3 *rot_axis, float rot_angle)
{
	glm::vec4 tmp=glm::vec4(vertex->x, vertex->y, vertex->z, 1.0);

	//glm::mat4 id();
	//glm::mat4 rot=glm::rotate(&id, rot_angle, rot_axis);
	//tmp=rot*tmp;

	glm::vec4 pos=glm::vec4(mesh_pos->x, mesh_pos->y, mesh_pos->z, 0);
	tmp=tmp+pos;

	glm::vec4 out=perspective_mat * tmp;
	
	//perspective divide and offset for screen, because of engine intern clipspace
	vertex->x=out.x/(tmp.z)+0.5;
	vertex->y=out.y/(tmp.z)+0.5;
	vertex->z=out.z/(tmp.z);
}