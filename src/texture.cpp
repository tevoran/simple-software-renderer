#include "ssr.hpp"

#include <glm/glm.hpp>
#include <loadbmp.h>

#include <iostream>

#define SSR_RGB 3

ssr::texture::texture(const char *path)
{
	loadbmp_decode_file(path, &pixel_data, &x, &y, LOADBMP_RGB);
}

void ssr::renderer::texture_map(struct ssr::pixel *data, const struct ssr::texture *texture)
{
	unsigned int tex_u=-0.5+(float)(texture->x)*data->u;
	unsigned int tex_v=-0.5+(float)(texture->y)*data->v;

	uint32_t offset=(tex_u+tex_v*texture->x)*SSR_RGB;
	data->b=texture->pixel_data[offset];
	data->r=texture->pixel_data[offset+1];
	data->g=texture->pixel_data[offset+2];

	/*if(data->g==255 && data->b==255 && data->r==255)
	{
		std::cout << data->x << "x" << data->y << std::endl;
		std::cout << "u: " << data->u << "  v: " << data->v << std::endl;
		exit(0);
	}*/

}