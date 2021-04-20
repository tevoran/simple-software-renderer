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
void ssr::renderer::raster_triangle(struct ssr::vertex *vertex1, struct ssr::vertex *vertex2, struct ssr::vertex *vertex3, uint32_t flags)
{
	//end renderering, if vertices are beyond far plane
	if(vertex1->z>1 && vertex2->z>1 && vertex3->z>1)
	{
		return;
	}

	//sorting vertices from lowest y-value to highest
	if(vertex1->y>vertex2->y)
	{
		std::swap(vertex1, vertex2);
	}
	if(vertex2->y>vertex3->y)
	{
		std::swap(vertex2, vertex3);
	}
	if(vertex1->y>vertex2->y)
	{
		std::swap(vertex1, vertex2);
	}


	//initializing necessary render variables
	glm::ivec3 vex1=glm::ivec3((float)vertex1->x*backbuffer->w, (float)vertex1->y*backbuffer->h, (float)vertex1->z*INT32_MAX);
	glm::ivec3 vex2=glm::ivec3((float)vertex2->x*backbuffer->w, (float)vertex2->y*backbuffer->h, (float)vertex2->z*INT32_MAX);
	glm::ivec3 vex3=glm::ivec3((float)vertex3->x*backbuffer->w, (float)vertex3->y*backbuffer->h, (float)vertex3->z*INT32_MAX);

	//end rendering if vertices are behind near plane
	if(vex1.z<0 && vex2.z<0 && vex3.z<0)
	{
		return;
	}

	//small triangle (pixel sized)
	if(vex2.x-vex1.x==0)
	{
		vex2.x++;
	}
	if(vex3.x-vex1.x==0)
	{
		vex3.x++;
	}
	if(vex3.y-vex1.y==0)
	{
		vex3.y++;
	}

	//Z-Buffering stuff
	int64_t dz_dy=(vex3.z-vex1.z)/(vex3.y-vex1.y);
	int64_t dz_dx;

	if((vex3.x-vex1.x)*(vex3.x-vex1.x)>(vex2.x-vex1.x)*(vex2.x-vex1.x))
	{
		dz_dx=(vex3.z-vex1.z)/(vex3.x-vex1.x);
	}
	else
	{
		dz_dx=(vex2.z-vex1.z)/(vex2.x-vex1.x);
	}


	//draw in wireframe mode
	if(flags==SSR_WIREFRAME)
	{
		//drawing in wireframe mode
		raster_line(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex2.x, vex2.y), vertex1->r, vertex1->g, vertex1->b);
		raster_line(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex3.x, vex3.y), vertex2->r, vertex2->g, vertex2->b);
		raster_line(glm::ivec2(vex2.x,vex2.y), glm::ivec2(vex3.x, vex3.y), vertex3->r, vertex3->g, vertex3->b);
		return;
	}


	//drawing in fill mode
	if(flags==SSR_FILL)
	{
		ssr::renderer::triangle_line_rendering line1(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex3.x, vex3.y)); //line 1 (vex1-3)
		ssr::renderer::triangle_line_rendering line2(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex2.x, vex2.y)); //line 2 (vex1-2)
		ssr::renderer::triangle_line_rendering line3(glm::ivec2(vex2.x,vex2.y), glm::ivec2(vex3.x, vex3.y)); //line 3 (vex2-3)

		struct ssr::pixel pixel;
		pixel.r=vertex1->r;
		pixel.g=vertex1->g;
		pixel.b=vertex1->b;
		pixel.y=vex1.y;

		uint64_t z_current_line=vex1.z;

		//upper part of the triangle
		do
		{
			line1.triangle_line_iterate();
			pixel.x=line1.get_x();
			pixel.y++;
			z_current_line+=dz_dy;

			line2.triangle_line_iterate();

			//draw line
			if(pixel.y>=0)
			{
				int32_t x_end=line2.get_x();
				if(pixel.x>x_end)
				{
					std::swap(pixel.x, x_end);
				}

				if(x_end>=res_x)
				{
					x_end=res_x-1;
				}

				if(pixel.x<0)
				{
					pixel.x=0;
				}
				pixel.z=z_current_line+(pixel.x-vex1.x)*dz_dx;

				/*uint32_t *pixel_ptr;
				pixel_ptr=(uint32_t*)(backbuffer->pixels)+(pixel.y*res_x+pixel.x)*sizeof(uint32_t);
				uint64_t *z_buffer_ptr;
				z_buffer_ptr=z_buffer+(pixel.y*res_x+pixel.x)*sizeof(uint64_t);*/
				do
				{
					draw_pixel(&pixel);
					pixel.z+=dz_dx;
					pixel.x++;
				}
				while(pixel.x<=x_end);
			}
		}
		while(pixel.y<vex2.y);
		
		//lower part of the triangle
		do
		{
			line1.triangle_line_iterate();
			pixel.x=line1.get_x();
			pixel.y++;
			z_current_line+=dz_dy;

			line3.triangle_line_iterate();

			//draw line
			if(pixel.y>=0)
			{
				int32_t x_end=line3.get_x();
				if(pixel.x>x_end)
				{
					std::swap(pixel.x, x_end);
				}

				if(x_end>=res_x)
				{
					x_end=res_x-1;
				}

				if(pixel.x<0)
				{
					pixel.x=0;
				}
				pixel.z=z_current_line+(pixel.x-vex1.x)*dz_dx;
				do
				{
					draw_pixel(&pixel);
					pixel.z+=dz_dx;
					pixel.x++;
				}
				while(pixel.x<=x_end);
			}
		}
		while(pixel.y<=vex3.y && pixel.y<res_y);
	}
}