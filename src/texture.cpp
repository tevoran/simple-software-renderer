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
	unsigned int tex_u=(float)(texture->x)*data->u;
	unsigned int tex_v=(float)(texture->y)*data->v;

	uint32_t offset=(tex_u+tex_v*texture->x)*SSR_RGB;
	data->b=texture->pixel_data[offset];
	data->r=texture->pixel_data[offset+1];
	data->g=texture->pixel_data[offset+2];


}