#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <math.h>
#include <exception>
#include <algorithm>
#include <limits>

//bresenham algorithm implementation
void ssr::renderer::raster_line(glm::ivec2 start, glm::ivec2 end, uint8_t r, uint8_t g, uint8_t b)
{
	//initializing variables
	int32_t bresenham_x=start.x;
	int32_t bresenham_y=start.y;
	int32_t bresenham_dx=end.x-start.x;
	int32_t bresenham_dy=end.y-start.y;
	int32_t bresenham_endx=end.x;
	int32_t bresenham_endy=end.y;

	//first pixel
	struct ssr::pixel pixel;
	pixel.x=start.x;
	pixel.y=start.y;
	pixel.r=r;
	pixel.g=g;
	pixel.b=b;
	pixel.z=0;
	draw_pixel(&pixel);

	//regular cases
		int32_t fast;
		int32_t fast_end;
		int32_t fast_d;
		int32_t slow;
		int32_t slow_d;

		//drawing location variables
		int32_t fast_x_choice;
		int32_t fast_y_choice;

		//fast x
		if(bresenham_dx*bresenham_dx>=bresenham_dy*bresenham_dy)
		{
			fast=bresenham_x;
			fast_end=bresenham_endx;
			fast_d=bresenham_dx;
			slow=bresenham_y;
			slow_d=bresenham_dy;
			fast_x_choice=1;
			fast_y_choice=0;
		}

		//fast y
		if(bresenham_dx*bresenham_dx<bresenham_dy*bresenham_dy)
		{
			fast=bresenham_y;
			fast_end=bresenham_endy;
			fast_d=bresenham_dy;
			slow=bresenham_x;
			slow_d=bresenham_dx;
			fast_x_choice=0;
			fast_y_choice=1;
		}

		//using multiplications instead of if's
		int32_t fast_cond;
		int32_t slow_cond;

		if(fast_d>=0)
		{
			fast_cond=1;
		}
		else
		{
			fast_cond=-1;
		}

		if(slow_d>=0)
		{
			slow_cond=-1;
		}
		else
		{
			slow_cond=1;
		}


			int32_t bresenham_error=fast_d/2;

				while(fast!=fast_end)
				{

					fast=fast+fast_cond;
					bresenham_error=bresenham_error+slow_cond*slow_d;

					if(bresenham_error<=0)
					{
						bresenham_error=bresenham_error+fast_cond*fast_d;
						slow=slow-slow_cond;
					}
					pixel.x=fast*fast_x_choice+slow*fast_y_choice;
					pixel.y=slow*fast_x_choice+fast*fast_y_choice;
					draw_pixel(&pixel);

				}
}

//using two instances of the bresenham algorithm to draw the triangle onto the screen
//drawing a line from vertex1 to vertex2 while drawing a line from vertex1 to vertex3
//in a second instance there is drawn a line from each pixel to corresponding pixel in
//the second line
void ssr::renderer::raster_triangle(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3, uint32_t flags)
{
	//end renderering, if vertices are beyond far plane
	if(vertex1.z>1 && vertex2.z>1 && vertex3.z>1)
	{
		return;
	}

	//sorting vertices from lowest y-value to highest
	if(vertex1.y>vertex2.y)
	{
		std::swap(vertex1, vertex2);
	}
	if(vertex2.y>vertex3.y)
	{
		std::swap(vertex2, vertex3);
	}
	if(vertex1.y>vertex2.y)
	{
		std::swap(vertex1, vertex2);
	}


	//initializing necessary render variables
	glm::ivec3 vex1=glm::ivec3((float)vertex1.x*backbuffer->w, (float)vertex1.y*backbuffer->h, (float)vertex1.z*INT32_MAX);
	glm::ivec3 vex2=glm::ivec3((float)vertex2.x*backbuffer->w, (float)vertex2.y*backbuffer->h, (float)vertex2.z*INT32_MAX);
	glm::ivec3 vex3=glm::ivec3((float)vertex3.x*backbuffer->w, (float)vertex3.y*backbuffer->h, (float)vertex3.z*INT32_MAX);

	//end rendering if vertices are behind near plane
/*	std::cout << "z: " << vex1.z << std::endl;
	std::cout << "z: " << vex2.z << std::endl;
	std::cout << "z: " << vex3.z << std::endl << std::endl;*/

	if(vex1.z<0 && vex2.z<0 && vex3.z<0)
	{
		return;
	}

	//calculating plain normal and stuff for z-buffering
		//base vector is vex1
		glm::vec3 u=glm::vec3(vertex2.x-vertex1.x, vertex2.y-vertex1.y, vertex2.z-vertex1.z);
		glm::vec3 v=glm::vec3(vertex3.x-vertex1.x, vertex3.y-vertex1.y, vertex3.z-vertex1.z);
		glm::vec3 n=glm::vec3(glm::cross(u, v)); //plain normal vector

		float c=n.z;
		float a=n.x/c;
		float b=n.y/c;
		float d=-n.x*vertex1.x-n.y*vertex1.y-n.z*vertex1.z;
		d=d/c;

		float z_x_increment=1/(float)res_x;
		float z_y_increment=1/(float)res_y;

	//draw in wireframe mode
	if(flags==SSR_WIREFRAME)
	{
		//drawing in wireframe mode
		raster_line(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex2.x, vex2.y), vertex1.r, vertex1.g, vertex1.b);
		raster_line(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex3.x, vex3.y), vertex2.r, vertex2.g, vertex2.b);
		raster_line(glm::ivec2(vex2.x,vex2.y), glm::ivec2(vex3.x, vex3.y), vertex3.r, vertex3.g, vertex3.b);
		return;
	}


	//drawing in fill mode
	if(flags==SSR_FILL)
	{
		ssr::renderer::triangle_line_rendering line1(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex3.x, vex3.y)); //line 1 (vex1-3)
		ssr::renderer::triangle_line_rendering line2(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex2.x, vex2.y)); //line 2 (vex1-2)
		ssr::renderer::triangle_line_rendering line3(glm::ivec2(vex2.x,vex2.y), glm::ivec2(vex3.x, vex3.y)); //line 3 (vex2-3)

		struct ssr::pixel pixel;
		pixel.r=vertex1.r;
		pixel.g=vertex1.g;
		pixel.b=vertex1.b;
		pixel.y=vex1.y;

		//upper part of the triangle
		do
		{
			line1.triangle_line_iterate();
			pixel.x=line1.get_x();
			pixel.y++;
			pixel.z=get_z(a, b, d, (float)pixel.x/(float)res_x, (float)pixel.y/(float)res_y);

			line2.triangle_line_iterate();

			//draw line
			float dz=get_z(a, b, d, (float)pixel.x/(float)res_x+z_x_increment, (float)pixel.y/(float)res_y)-pixel.z;
			int32_t x_end=line2.get_x();
			do
			{
				pixel.z+=dz;
				pixel.x++;
				draw_pixel(&pixel);
			}
			while(pixel.x<=x_end);
		}
		while(pixel.y<vex2.y);
		
		//lower part of the triangle
		do
		{
			line1.triangle_line_iterate();
			pixel.x=line1.get_x();
			pixel.y++;
			pixel.z=get_z(a, b, d, (float)pixel.x/(float)res_x, (float)pixel.y/(float)res_y);


			line3.triangle_line_iterate();

			float dz=get_z(a, b, d, (float)pixel.x/(float)res_x+z_x_increment, (float)pixel.y/(float)res_y)-pixel.z;
			int32_t x_end=line3.get_x();
			do
			{
				pixel.z+=dz;
				pixel.x++;
				draw_pixel(&pixel);
			}
			while(pixel.x<=x_end);
		}
		while(pixel.y<=vex3.y);
	}
}