#include "ssr.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#define SSR_RGB 3
void ssr::renderer::texture_map(struct ssr::pixel *data, const struct ssr::texture *texture)
{
	unsigned int tex_u=(float)(texture->x)*data->u;
	unsigned int tex_v=(float)(texture->y)*data->v;

	uint32_t offset=(tex_u+tex_v*texture->x)*SSR_RGB;
	if(offset>(texture->y*texture->x)*SSR_RGB)
	{
		std::cout << "UxV: " << tex_u << " x " << tex_v << std::endl;
		exit(0);
		return;
	}
	data->b=texture->pixel_data[offset];
	data->r=texture->pixel_data[offset+1];
	data->g=texture->pixel_data[offset+2];

}