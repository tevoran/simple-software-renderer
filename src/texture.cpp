#include "ssr.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

void ssr::renderer::texture_map(struct ssr::pixel *data, struct ssr::texture texture, float u, float v)
{
	int tex_u=(float)(texture.x-1)*data->u;
	int tex_v=(float)(texture.y-1)*data->v;

	//std::cout << "UxV: " << data->u << "x" << data->v <<std::endl;
	//std::cout << "UxV: " << tex_u << "x" << tex_v <<std::endl << std::endl;

	register uint32_t color_tmp=texture.pixel_data[tex_u+tex_v*texture.x];
	data->b=color_tmp&0xFF;
	color_tmp=color_tmp>>8;
	data->g=color_tmp&0xFF;
	color_tmp=color_tmp>>8;
	data->r=color_tmp&0xFF;
}