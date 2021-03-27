#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <math.h>
#include <exception>
#include <algorithm>

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
	draw_pixel(start.x, start.y, r, g, b);


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

					draw_pixel(fast*fast_x_choice+slow*fast_y_choice, slow*fast_x_choice+fast*fast_y_choice, r, g, b);

				}
}

//using two instances of the bresenham algorithm to draw the triangle onto the screen
//drawing a line from vertex1 to vertex2 while drawing a line from vertex1 to vertex3
//in a second instance there is drawn a line from each pixel to corresponding pixel in
//the second line
void ssr::renderer::raster_triangle(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3)
{
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

	std::cout << "vertex1: y:" << vertex1.y << std::endl;
	std::cout << "vertex2: y:" << vertex2.y << std::endl;
	std::cout << "vertex3: y:" << vertex3.y << std::endl;


	//line vertex1-vertex2
	//initializing variables
	int32_t bresenham_x=(float)vertex1.x*backbuffer->w;
	int32_t bresenham_y=(float)vertex1.y*backbuffer->h;
	int32_t bresenham_dx=((float)vertex2.x-(float)vertex1.x)*backbuffer->w;
	int32_t bresenham_dy=((float)vertex2.y-(float)vertex1.y)*backbuffer->h;
	int32_t bresenham_endx=(float)vertex2.x*backbuffer->w;
	int32_t bresenham_endy=(float)vertex2.y*backbuffer->h;

	glm::ivec2 vertex3_draw=glm::ivec2((float)vertex3.x*backbuffer->w,(float)vertex3.y*backbuffer->h);

	glm::ivec2 start=glm::ivec2(bresenham_x, bresenham_y);
	glm::ivec2 end=glm::ivec2(bresenham_endx, bresenham_endy);
	//raster_line(start, end, 255, 255, 255);

	//first pixel
	draw_pixel(start.x, start.y, 255, 255, 255);


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

					glm::ivec2 start=glm::ivec2(fast*fast_x_choice+slow*fast_y_choice, slow*fast_x_choice+fast*fast_y_choice);
					raster_line(start, vertex3_draw, 255, 255, 255);

				}
}