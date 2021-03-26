#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <math.h>
#include <exception>

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
		z_buffer=new float[backbuffer->w * backbuffer->h];

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

void ssr::renderer::clear(uint8_t r, uint8_t g, uint8_t b)
{
	//RGB pixel format
	//highest byte is red, lowest byte is blue
	if(pixel_type == SDL_PIXELFORMAT_RGB888)
	{
		uint32_t clear_color = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGB888), r, g, b);
		uint32_t *pixel_ptr = static_cast<uint32_t*>(backbuffer->pixels);
		for(int i=0; i<(backbuffer->w*backbuffer->h); i++)
		{
			pixel_ptr[i]=clear_color;
		}
	}
	else
	{
		throw "SDL: unknown pixel format and cannot clear the screen";
	}

}

void ssr::renderer::draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	if(x<(backbuffer->w) && x>=0 && y>=0 && y<(backbuffer->h))
	{
		//RGB pixel format
		//highest byte is red, lowest byte is blue
		if(pixel_type == SDL_PIXELFORMAT_RGB888)
		{
				uint32_t *pixel_ptr = static_cast<uint32_t*>(backbuffer->pixels);
				uint32_t pixel_colored = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGB888), r, g, b);
				pixel_ptr[x+y*(backbuffer->w)]=pixel_colored;
		}
		else
		{
			throw "SDL: unknown pixel format and cannot draw a pixel";
		}
	}
}

/*the renderer uses a clip space that is similar to OpenGL. But the clip space's borders are
0 and 1 along the different axes.*/
void ssr::renderer::render(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3)
{
	//counting the rendered vertices
	static unsigned int num_vertices = 0;

	//show vertex data
	/*num_vertices++;
	std::cout << "render vertex " << num_vertices << std::endl;
	std::cout << "location: " << vertex1.x << " " << vertex1.y << " " << vertex1.z << std::endl;
	std::cout << "color: " << (int)vertex1.r << " " << (int)vertex1.g << " " << (int)vertex1.b << std::endl;

	num_vertices++;
	std::cout << "render vertex " << num_vertices << std::endl;
	std::cout << "location: " << vertex2.x << " " << vertex2.y << " " << vertex2.z << std::endl;
	std::cout << "color: " << (int)vertex2.r << " " << (int)vertex2.g << " " << (int)vertex2.b << std::endl;*/

	//draw a line with the Bresenham algorithm
	/*int32_t bresenham_x=(float)vertex1.x*backbuffer->w;
	int32_t bresenham_y=(float)vertex1.y*backbuffer->h;
	int32_t bresenham_dx=((float)vertex2.x-(float)vertex1.x)*backbuffer->w;
	int32_t bresenham_dy=((float)vertex2.y-(float)vertex1.y)*backbuffer->h;
	int32_t bresenham_endx=(float)vertex2.x*backbuffer->w;
	int32_t bresenham_endy=(float)vertex2.y*backbuffer->h;

		//deciding which is the fast axis
		//fast x
		if(bresenham_dx*bresenham_dx > bresenham_dy*bresenham_dy)
		{
			int32_t bresenham_error=bresenham_dx/2;

			//first pixel
			draw_pixel(bresenham_x, bresenham_y, vertex1.r, vertex1.g, vertex1.b);

			std::cout << "dy: " << bresenham_dy << std::endl;
			while(bresenham_x!=bresenham_endx)
			{
				bresenham_x++;
				bresenham_error-=bresenham_dy;
				std::cout << bresenham_x << "x" << bresenham_y << " error: " << bresenham_error << std::endl;
				if(bresenham_error<0)
					{
						bresenham_y++;
						bresenham_error+=bresenham_dx;
					}
				draw_pixel(bresenham_x, bresenham_y, vertex1.r, vertex1.g, vertex1.b);
			}

		}

		//fast y
		if(bresenham_dx*bresenham_dx < bresenham_dy*bresenham_dy)
		{
			std::cout << "lolo" << std::endl;
		}
	*/
	raster_triangle(vertex1, vertex2, vertex3);





	//vertex transformation aka vertex shader
	glm::vec4 vex={vertex1.x, vertex1.y, vertex1.z, 0};


	//rasterization of the pixel
	int32_t x,y;
	x=(float)vex.x*(backbuffer->w);
	y=(float)vex.y*(backbuffer->h);


	/*std::cout << "XxY: " << vex.x << " " << vex.y << std::endl;
	std::cout << "XxY (rasterized): " << x << "x" << y << std::endl;*/

	//draw_pixel(x,y,vertex1.r, vertex1.g, vertex1.b);

	//render changed backbuffer onto the screen
	if(SDL_UpdateWindowSurface(window)!=0)
	{
		std::cout << "SDL ERROR MSG: " << std::endl << SDL_GetError() << std::endl;
	}
	return;	
}