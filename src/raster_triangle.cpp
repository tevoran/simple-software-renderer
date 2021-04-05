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
void ssr::renderer::raster_triangle(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3, uint32_t flags)
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

	//std::cout << "vertex1: y:" << vertex1.y << std::endl;
	//std::cout << "vertex2: y:" << vertex2.y << std::endl;
	//std::cout << "vertex3: y:" << vertex3.y << std::endl;


	//initializing necessary render variables
	int32_t vertex1_render_x=(float)vertex1.x*backbuffer->w;
	int32_t vertex1_render_y=(float)vertex1.y*backbuffer->h;

	int32_t vertex2_render_x=(float)vertex2.x*backbuffer->w;
	int32_t vertex2_render_y=(float)vertex2.y*backbuffer->h;

	int32_t vertex3_render_x=(float)vertex3.x*backbuffer->w;
	int32_t vertex3_render_y=(float)vertex3.y*backbuffer->h;

	glm::ivec2 vex1=glm::ivec2(vertex1_render_x, vertex1_render_y);
	glm::ivec2 vex2=glm::ivec2(vertex2_render_x, vertex2_render_y);
	glm::ivec2 vex3=glm::ivec2(vertex3_render_x, vertex3_render_y);


	if(flags==SSR_WIREFRAME)
	{
		//drawing in wireframe mode
		raster_line(vex1, vex2, 255, 255, 255);
		raster_line(vex1, vex3, 255, 255, 255);
		raster_line(vex2, vex3, 255, 255, 255);
		return;
	}

	//drawing in fill mode


	if(flags==SSR_FILL)
	{
		ssr::renderer::triangle_line_rendering line1(vex1, vex3); //line 1 (vex1-3)
		ssr::renderer::triangle_line_rendering line2(vex1, vex2); //line 2 (vex1-2)
		ssr::renderer::triangle_line_rendering line3(vex2, vex3); //line 3 (vex2-3)

		while(line1.line_done()!=true)
		{
			line1.triangle_line_iterate();
			draw_pixel(line1.get_location().x, line1.get_location().y,255,255,255);
			//line1.show_variables();
			//char n;
			//std::cin >> n;
		}

		while(line2.line_done()!=true)
		{
			line2.triangle_line_iterate();
			draw_pixel(line2.get_location().x, line2.get_location().y,255,255,255);
		}

		while(line3.line_done()!=true)
		{
			line3.triangle_line_iterate();
			draw_pixel(line3.get_location().x, line3.get_location().y,255,255,255);
		}
	}
}