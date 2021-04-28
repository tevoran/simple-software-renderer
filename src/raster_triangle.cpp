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
void ssr::renderer::raster_triangle(struct ssr::vertex *vertex1, struct ssr::vertex *vertex2, struct ssr::vertex *vertex3, const ssr::texture *texture, uint32_t flags)
{
	/*checking if triangle actually needs to be rasterized*/
	//end renderering, if vertices are beyond far plane
	if(vertex1->z>1 && vertex2->z>1 && vertex3->z>1)
	{
		return;
	}

	//end rendering if vertices are behind near plane
	if(vertex1->z<0 && vertex2->z<0 && vertex3->z<0)
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


	//getting renderspace (screenspace) coordinates
	glm::ivec3 vex1=glm::ivec3((float)vertex1->x*backbuffer->w, (float)vertex1->y*backbuffer->h, (float)vertex1->z*INT32_MAX);
	glm::ivec3 vex2=glm::ivec3((float)vertex2->x*backbuffer->w, (float)vertex2->y*backbuffer->h, (float)vertex2->z*INT32_MAX);
	glm::ivec3 vex3=glm::ivec3((float)vertex3->x*backbuffer->w, (float)vertex3->y*backbuffer->h, (float)vertex3->z*INT32_MAX);


	//draw in wireframe mode
	if(flags==SSR_WIREFRAME)
	{
		//drawing in wireframe mode
		raster_line(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex2.x, vex2.y), 255, 255, 255);
		raster_line(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex3.x, vex3.y), 255, 255, 255);
		raster_line(glm::ivec2(vex2.x,vex2.y), glm::ivec2(vex3.x, vex3.y), 255, 255, 255);
		return;
	}


	//drawing in fill mode
	if(flags==SSR_FILL)
	{

		//avoiding divisions by zero
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

		ssr::renderer::triangle_line_rendering line1(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex3.x, vex3.y)); //line 1 (vex1-3)
		ssr::renderer::triangle_line_rendering line2(glm::ivec2(vex1.x,vex1.y), glm::ivec2(vex2.x, vex2.y)); //line 2 (vex1-2)
		ssr::renderer::triangle_line_rendering line3(glm::ivec2(vex2.x,vex2.y), glm::ivec2(vex3.x, vex3.y)); //line 3 (vex2-3)

		struct ssr::pixel pixel;
		pixel.r=vertex1->r;
		pixel.g=vertex1->g;
		pixel.b=vertex1->b;
		pixel.y=vex1.y;

		//texture calculation
		int64_t cramer_div=((vex2.x-vex1.x)*(vex3.y-vex2.y)-(vex2.y-vex1.y)*(vex3.x-vex2.x));
		if(cramer_div==0)
		{
			return;
		}

		uint64_t z_current_line=vex1.z;

		//upper part of the triangle
		while(pixel.y<vex2.y)
		{
			pixel.x=line1.triangle_line_iterate();
			pixel.y++;
			z_current_line+=dz_dy;

			int32_t x_end=line2.triangle_line_iterate();

			//draw line
			if(pixel.y>=0)
			{
				if(pixel.x>x_end)
				{
					std::swap(pixel.x, x_end);
				}

				if(pixel.x>=res_x)
				{
					continue;
				}

				if(x_end<0)
				{
					continue;
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

				//texture calculation optimizations
				int32_t vex1_dy=vex1.y-pixel.y;
				int32_t vex2_dy=vex2.y-pixel.y;
				int32_t vex3_dy=vex3.y-pixel.y;

				int32_t m1_dx=vex2_dy-vex3_dy;
				int64_t m1_const=vex2.x*vex3_dy-vex3.x*vex2_dy;
				int32_t m1_line=m1_const+m1_dx*pixel.x;

				int32_t m2_dx=vex3_dy-vex1_dy;
				int32_t m2_const=vex3.x*vex1_dy-vex1.x*vex3_dy;
				int32_t m2_line=m2_const+m2_dx*pixel.x;


				//the actual number of the pixel that is drawn
				uint32_t pixel_offset;
				pixel_offset=pixel.x+pixel.y*backbuffer->w;
				while(pixel.x<=x_end && pixel_offset<(res_x*res_y))
				{
					if(pixel.z<z_buffer[pixel_offset])
					{
						//texture calculation
						//calculating u-v-coordinates
						{
							m1_line+=m1_dx;
							m2_line+=m2_dx;

							//std::cout << "m1_line: " << m1_line << std::endl;
							//std::cout << "cramer_div: " << cramer_div << std::endl;
							float m1=(float)m1_line/(float)cramer_div;
							float m2=(float)m2_line/(float)cramer_div;
							float m3=1-m1-m2;

							pixel.u=(float)m1*vertex1->u+(float)m2*vertex2->u+(float)m3*vertex3->u;
							pixel.v=(float)m1*vertex1->v+(float)m2*vertex2->v+(float)m3*vertex3->v;

							//std::cout << "vertex1: " << vertex1->u << "x" << vertex1->v << std::endl;
							//std::cout << "vertex2: " << vertex2->u << "x" << vertex2->v << std::endl;
							//std::cout << "vertex3: " << vertex3->u << "x" << vertex3->v << std::endl;

							//std::cout << m1 << "x" << m2 << "x" << m3 << std::endl;
							//std::cout << pixel.u << "x" << pixel.v << std::endl;
							pixel.u=abs(pixel.u-(int)pixel.u);
							pixel.v=abs(pixel.v-(int)pixel.v);
							//std::cout << pixel.u << "x" << pixel.v << std::endl;
							//SDL_Delay(1000);
						}
						texture_map(&pixel, texture);
						draw_pixel_fast(&pixel, pixel_offset);						
					}
					pixel_offset++;

					pixel.z+=dz_dx;
					pixel.x++;
				}
			}
		}
		
		//lower part of the triangle
		while(pixel.y<=vex3.y && pixel.y<(res_y-1))
		{
			pixel.x=line1.triangle_line_iterate();
			pixel.y++;
			z_current_line+=dz_dy;


			int32_t x_end=line3.triangle_line_iterate();

			//draw line
			if(pixel.y>=0)
			{
				if(pixel.x>x_end)
				{
					std::swap(pixel.x, x_end);
				}

				if(pixel.x>=res_x)
				{
					continue;
				}

				if(x_end<0)
				{
					continue;
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
				
				//texture calculation optimizations
				int32_t vex1_dy=vex1.y-pixel.y;
				int32_t vex2_dy=vex2.y-pixel.y;
				int32_t vex3_dy=vex3.y-pixel.y;

				int32_t m1_dx=vex2_dy-vex3_dy;
				int32_t m1_const=vex2.x*vex3_dy-vex3.x*vex2_dy;
				int32_t m1_line=m1_const+m1_dx*pixel.x;

				int32_t m2_dx=vex3_dy-vex1_dy;
				int32_t m2_const=vex3.x*vex1_dy-vex1.x*vex3_dy;
				int32_t m2_line=m2_const+m2_dx*pixel.x;

				//the actual number of the pixel that is drawn
				uint32_t pixel_offset;
				pixel_offset=pixel.x+pixel.y*backbuffer->w;
				while(pixel.x<=x_end && pixel_offset<(res_x*res_y))
				{
					if(pixel.z<z_buffer[pixel_offset])
					{
						//texture calculation
						//calculating u-v-coordinates
						{
							m1_line+=m1_dx;
							m2_line+=m2_dx;
							float m1=(float)m1_line/(float)cramer_div;
							float m2=(float)m2_line/(float)cramer_div;
							float m3=1-m1-m2;

							pixel.u=(float)m1*vertex1->u+(float)m2*vertex2->u+(float)m3*vertex3->u;
							pixel.v=(float)m1*vertex1->v+(float)m2*vertex2->v+(float)m3*vertex3->v;

							//std::cout << m1 << "x" << m2 << "x" << m3 << std::endl;
							//std::cout << pixel.u << "x" << pixel.v << std::endl;
							pixel.u=abs(pixel.u-(int)pixel.u);
							pixel.v=abs(pixel.v-(int)pixel.v);
							//std::cout << pixel.u << "x" << pixel.v << std::endl;
							//SDL_Delay(1000);
						}
						texture_map(&pixel, texture);
						draw_pixel_fast(&pixel, pixel_offset);						
					}
					pixel_offset++;

					pixel.z+=dz_dx;
					pixel.x++;
				}
			}
		}
	}
}