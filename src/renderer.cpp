#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <exception>
#include <limits.h>


ssr::renderer::renderer()
{
	//SDL INIT
	if(SDL_Init(SDL_INIT_EVERYTHING)==0)
		{
			std::cout << "SDL successfully initialized" << std::endl;
		}
		else
		{
			throw "SDL ERROR while initializing SDL";
		}

	//SDL creating window
	window = SDL_CreateWindow(	"Simple Software Renderer - by Johannes Pfeiffer",
											SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											res_x, 
											res_y, 
											0);
		if(window==NULL)
		{
			throw "SDL couldn't create window";
		}

	//creating render surfaces
	backbuffer = SDL_GetWindowSurface(window);
		if(backbuffer==NULL)
		{
			throw "SDL couldn't create render surface";
		}
		else
		{
			//getting pixel format and setting pixel format variable accordingly
			pixel_type = backbuffer->format->format;
			std::cout << "Format: " << SDL_GetPixelFormatName(pixel_type) << std::endl;
		}

	//renderer initialization
		//Z-Buffer initialization
		z_buffer=new uint64_t[backbuffer->w * backbuffer->h];

		std::cout << "Resolution: " << backbuffer->w << "x" << backbuffer->h << std::endl;
		memset(z_buffer, 0xFF, backbuffer->h*backbuffer->w*sizeof(uint64_t));


		//Projection matrix
		perspective_mat[0].x=tan(0.5*PI-0.5*fov);
		perspective_mat[0].y=0;
		perspective_mat[0].z=0;
		perspective_mat[0].w=0;

		perspective_mat[1].x=0;
		perspective_mat[1].y=1;
		perspective_mat[1].z=0;
		perspective_mat[1].w=0;

		perspective_mat[2].x=0;
		perspective_mat[2].y=0;
		perspective_mat[2].z=1;
		perspective_mat[2].w=0;

		perspective_mat[3].x=0;
		perspective_mat[3].y=0;
		perspective_mat[3].z=0;
		perspective_mat[3].w=1;

}

ssr::renderer::~renderer()
{
	delete [] z_buffer;
}

//clears the background to black
void ssr::renderer::update()
{
	//render changed backbuffer onto the screen
	if(SDL_UpdateWindowSurface(window)!=0)
	{
		std::cout << "SDL ERROR MSG: " << std::endl << SDL_GetError() << std::endl;
	}

	//clearing screen
	static void *pixel_ptr = static_cast<void*>(backbuffer->pixels);
	memset(pixel_ptr, 0, (backbuffer->h)*(backbuffer->w)*sizeof(uint32_t));

	//clearing screen
	//RGB pixel format
	//highest byte is red, lowest byte is blue
	if(pixel_type != SDL_PIXELFORMAT_RGB888)
	{
		throw "SDL: unknown pixel format and cannot clear the screen";
	}
	else
	{
		//necessary if pixelformat is different, then the renderer needs to rewrite the buffer to
		//suit the pixelformant
	}


	//clearing z-buffer
	memset(z_buffer, 0xFF, backbuffer->h*backbuffer->w*sizeof(uint64_t)); //setting it to max value

}

//make sure your pixel is on screen
void ssr::renderer::draw_pixel(struct ssr::pixel *data)
{
	uint32_t pixel_offset=data->y*backbuffer->w+data->x;

	if(data->z<z_buffer[pixel_offset])
	{
		//write in PIXELFORMAT_RGB888
		static uint32_t *pixel_ptr = static_cast<uint32_t*>(backbuffer->pixels);
		uint32_t pixel_colored=data->r;
		pixel_colored=pixel_colored<<8;
		pixel_colored+=data->g;
		pixel_colored=pixel_colored<<8;
		pixel_colored+=data->b;
		pixel_ptr[pixel_offset]=pixel_colored;

		z_buffer[pixel_offset]=data->z;
	}
}


/*the renderer uses a clip space that is similar to OpenGL. But the clip space's borders are
0 and 1 along the different axes.*/
void ssr::renderer::render(struct vertex *data, uint32_t num_polygons, uint32_t flags)
{	
	static struct vertex *vertex1, *vertex2, *vertex3;

	for(int i=0; i<num_polygons; i++)
	{
		//vertex1
		vertex1=&data[i*3];
		vertex2=&data[i*3+1];
		vertex3=&data[i*3+2];

		//rasterization
		raster_triangle(*vertex1, *vertex2, *vertex3, flags);
	}







	//vertex transformation aka vertex shader
	//glm::vec4 vex={vertex1.x, vertex1.y, vertex1.z, 0};

	return;	
}