#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <math.h>
#include <exception>

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
	draw_pixel(bresenham_x, bresenham_y, r, g, b);


	//regular cases
		int32_t fast;
		int32_t fast_end;
		int32_t fast_d;
		int32_t slow;
		int32_t slow_d;
		//fast x
		if(bresenham_dx*bresenham_dx>=bresenham_dy*bresenham_dy)
		{
			fast=bresenham_x;
			fast_end=bresenham_endx;
			fast_d=bresenham_dx;
			slow=bresenham_y;
			slow_d=bresenham_dy;
		}

		//fast y
		if(bresenham_dx*bresenham_dx<bresenham_dy*bresenham_dy)
		{
			fast=bresenham_y;
			fast_end=bresenham_endy;
			fast_d=bresenham_dy;
			slow=bresenham_x;
			slow_d=bresenham_dx;
		}

			int32_t bresenham_error=fast_d/2;

				while(fast!=fast_end)
				{
					if(fast_d>=0)
						{
							fast++;
						}
						else
						{
							fast--;
						}

					if(slow_d>=0)
						{
							bresenham_error-=slow_d;
						}
						else
						{
							bresenham_error+=slow_d;
						}
					if(bresenham_error>1000)
					{
						exit(0);
					}
					if(bresenham_error<=0)
					{
						if(slow_d>=0)
							{
								if(fast_d>=0)
								{
									bresenham_error+=fast_d;
								}
								else
								{
									bresenham_error-=fast_d;
								}
								slow++;
							}
							else
							{
								if(fast_d>=0)
								{
									bresenham_error+=fast_d;
								}
								else
								{
									bresenham_error-=fast_d;
								}
								slow--;
							}
					}
					//fast x
					if(bresenham_dx*bresenham_dx>=bresenham_dy*bresenham_dy)
					{
						draw_pixel(fast, slow, r, g, b);
					}
					//fast y
					if(bresenham_dx*bresenham_dx<bresenham_dy*bresenham_dy)
					{
						draw_pixel(slow, fast, r, g, b);
					}

				}
}

//using two instances of the bresenham algorithm to draw the triangle onto the screen
//drawing a line from vertex1 to vertex2 while drawing a line from vertex1 to vertex3
//in a second instance there is drawn a line from each pixel to corresponding pixel in
//the second line
void ssr::renderer::raster_triangle(struct ssr::vertex vertex1, struct ssr::vertex vertex2, struct ssr::vertex vertex3)
{
	//line vertex1-vertex2
	//initializing variables
	int32_t bresenham_x=(float)vertex1.x*backbuffer->w;
	int32_t bresenham_y=(float)vertex1.y*backbuffer->h;
	int32_t bresenham_dx=((float)vertex2.x-(float)vertex1.x)*backbuffer->w;
	int32_t bresenham_dy=((float)vertex2.y-(float)vertex1.y)*backbuffer->h;
	int32_t bresenham_endx=(float)vertex2.x*backbuffer->w;
	int32_t bresenham_endy=(float)vertex2.y*backbuffer->h;

	glm::ivec2 start=glm::ivec2(bresenham_x, bresenham_y);
	glm::ivec2 end=glm::ivec2(bresenham_endx, bresenham_endy);
	raster_line(start, end, 255, 255, 255);

	/*
	//catching edgecases
		//line parallel to x-axis in positive direction
		if(bresenham_dy==0 && bresenham_dx>0)
		{
			while(bresenham_x<=bresenham_endx)
			{
				bresenham_x++;
				draw_pixel(bresenham_x,bresenham_y, vertex1.r, vertex1.g, vertex1.b);	
			}
			return;
		}

		//line parallel to x-axis in negative direction
		if(bresenham_dy==0 && bresenham_dx<0)
		{
			while(bresenham_x>=bresenham_endx)
			{
				bresenham_x--;
				draw_pixel(bresenham_x, bresenham_y, vertex1.r, vertex1.g, vertex1.b);
			}
		}
		*/
}