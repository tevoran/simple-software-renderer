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

	std::cout << "vertex1: y:" << vertex1.y << std::endl;
	std::cout << "vertex2: y:" << vertex2.y << std::endl;
	std::cout << "vertex3: y:" << vertex3.y << std::endl;


	//initializing necessary render variables
	int32_t vertex1_render_x=(float)vertex1.x*backbuffer->w;
	int32_t vertex1_render_y=(float)vertex1.y*backbuffer->h;

	int32_t vertex2_render_x=(float)vertex2.x*backbuffer->w;
	int32_t vertex2_render_y=(float)vertex2.y*backbuffer->h;

	int32_t vertex3_render_x=(float)vertex3.x*backbuffer->w;
	int32_t vertex3_render_y=(float)vertex3.y*backbuffer->h;

	if(flags==SSR_WIREFRAME)
	{
		//drawing in wireframe mode
		glm::ivec2 vertex1_outline=glm::ivec2(vertex1_render_x, vertex1_render_y);
		glm::ivec2 vertex2_outline=glm::ivec2(vertex2_render_x, vertex2_render_y);
		glm::ivec2 vertex3_outline=glm::ivec2(vertex3_render_x, vertex3_render_y);
		raster_line(vertex1_outline, vertex2_outline, 255, 255, 255);
		raster_line(vertex1_outline, vertex3_outline, 255, 255, 255);
		raster_line(vertex2_outline, vertex3_outline, 255, 255, 255);
		return;
	}

	//drawing in fill mode
	//line 1 (vertex1-vertex3)
	int32_t line1_dx=vertex3_render_x-vertex1_render_x;
	int32_t line1_dy=vertex3_render_y-vertex1_render_y; //can't be negative
		int32_t line1_s;
		int32_t line1_s_end;
		int32_t line1_ds;
		int32_t line1_t;
		int32_t line1_t_end;
		int32_t line1_dt;

		int32_t line1_error;
		int32_t line1_s_step=1;
		int32_t line1_t_step=1;
		
		bool line1_fastx=false;
		//fast x
			//s=x
			//t=y
		if(abs(line1_dx)>=line1_dy)
		{
			line1_s=vertex1_render_x;
			line1_t=vertex1_render_y;
			line1_s_end=vertex3_render_x;
			line1_t_end=vertex3_render_y;
			line1_fastx=true;

			if(line1_dx<0)
			{
				line1_s_step=-1;
			}
		}
		//fast y
			//t=x
			//s=y
		else
		{
			line1_t=vertex1_render_x;
			line1_s=vertex1_render_y;
			line1_t_end=vertex3_render_x;
			line1_s_end=vertex3_render_y;

			if(line1_dx<0)
			{
				line1_t_step=-1;
			}
		}

		line1_ds=line1_s_end-line1_s;
		line1_dt=line1_t_end-line1_t;
		line1_error=line1_s/2;

	//line 2 (vertex1-vertex2)
	int32_t line2_dx=vertex2_render_x-vertex1_render_x;
	int32_t line2_dy=vertex2_render_y-vertex1_render_y; //can't be negative
		int32_t line2_s;
		int32_t line2_s_end;
		int32_t line2_ds;
		int32_t line2_t;
		int32_t line2_t_end;
		int32_t line2_dt;

		int32_t line2_error;
		int32_t line2_s_step=1;
		int32_t line2_t_step=1;
		
		bool line2_fastx=false;
		//fast x
			//s=x
			//t=y
		if(abs(line2_dx)>=line2_dy)
		{
			line2_s=vertex1_render_x;
			line2_t=vertex1_render_y;
			line2_s_end=vertex2_render_x;
			line2_t_end=vertex2_render_y;
			line2_fastx=true;

			if(line2_dx<0)
			{
				line2_s_step=-1;
			}
		}
		//fast y
			//t=x
			//s=y
		else
		{
			line2_t=vertex1_render_x;
			line2_s=vertex1_render_y;
			line2_t_end=vertex2_render_x;
			line2_s_end=vertex2_render_y;

			if(line2_dx<0)
			{
				line2_t_step=-1;
			}
		}

		line2_ds=line2_s_end-line1_s;
		line2_dt=line2_t_end-line1_t;
		line2_error=line2_s/2;

	//line 3 (vertex2-vertex3)
	int32_t line3_dx=vertex3_render_x-vertex2_render_x;
	int32_t line3_dy=vertex3_render_y-vertex2_render_y; //can't be negative
		int32_t line3_s;
		int32_t line3_s_end;
		int32_t line3_ds;
		int32_t line3_t;
		int32_t line3_t_end;
		int32_t line3_dt;

		int32_t line3_error;
		int32_t line3_s_step=1;
		int32_t line3_t_step=1;
		
		bool line3_fastx=false;
		//fast x
			//s=x
			//t=y
		if(abs(line3_dx)>=line3_dy)
		{
			line3_s=vertex2_render_x;
			line3_t=vertex2_render_y;
			line3_s_end=vertex3_render_x;
			line3_t_end=vertex3_render_y;
			line3_fastx=true;

			if(line3_dx<0)
			{
				line3_s_step=-1;
			}
		}
		//fast y
			//t=x
			//s=y
		else
		{
			line3_t=vertex2_render_x;
			line3_s=vertex2_render_y;
			line3_t_end=vertex3_render_x;
			line3_s_end=vertex3_render_y;

			if(line3_dx<0)
			{
				line3_t_step=-1;
			}
		}

		line3_ds=line3_s_end-line3_s;
		line3_dt=line3_t_end-line3_t;
		line3_error=line3_s/2;


	if(flags==SSR_FILL)
	{
		bool loop_nop=false;
		bool loop_quit=false;
		bool line1_draw_y=false;
		bool line2_draw_y=false;
		bool line3_draw_y=false;
		
		while(loop_quit==false)
		{
			//line1 (vertex1-vertex3)
			if((line1_s!=line1_s_end) && (line1_draw_y==false))
			{
				loop_nop=false;
				//std::cout<< "1" << std::endl;
				line1_s+=line1_s_step;
				line1_error-=line1_dt;

				if(line1_fastx==false)
				{
					line1_draw_y=true;
				}

				if(line1_error<=0)
				{
					line1_error+=line1_ds;
					line1_t+=line1_t_step;
					if(line1_fastx==true)
					{
						line1_draw_y=true;
					}

				}
				
			}

			//line2 (vertex1-vertex2)
			if(line2_s!=line2_s_end && line2_draw_y==false && line2_ds!=0)
			{
				loop_nop=false;
				//std::cout<< "2" << std::endl;
				line2_s+=line2_s_step;
				line2_error-=line2_dt;

				if(line2_fastx==false)
				{
					line2_draw_y=true;
				}

				if(line2_error<=0)
				{
					line2_error+=line2_ds;
					line2_t+=line2_t_step;
					if(line2_fastx==true)
					{
						line2_draw_y=true;
					}

				}
				
			}

			//line3 (vertex2-vertex3)
			if(line3_s!=line3_s_end && line2_s==line2_s_end && line3_draw_y==false && line3_ds!=0)
			{
				loop_nop=false;
				//std::cout<< "3" << std::endl;
				line3_s+=line3_s_step;
				line3_error-=line3_dt;

				if(line3_fastx==false)
				{
					line3_draw_y=true;
				}
				if(line3_error<=0)
				{
					line3_error+=line1_ds;
					line3_t+=line1_t_step;
					if(line3_fastx==true)
					{
						line3_draw_y=true;
					}

				}
				
			}

			//draw y-axis-rows
			if(line1_draw_y==true && line2_draw_y==true)
			{
				loop_nop=false;
				//std::cout<< "4" << std::endl;
				line1_draw_y=false;
				line2_draw_y=false;

				int32_t x,y,x_end,y_end;
				if(line1_fastx==true)
				{
					x=line1_s;
					y=line1_t;
				}
				else
				{
					x=line1_t;
					y=line1_s;
				}
				if(line2_fastx==true)
				{
					x_end=line2_s;
					y_end=line2_t;
				}
				else
				{
					x_end=line2_t;
					y_end=line2_s;
				}
				int32_t x_step=0;
				if(x<x_end)
				{
					x_step=1;
				}
				if(x>x_end)
				{
					x_step=-1;
				}

				do
				{
					x+=x_step;
					draw_pixel(x, y, 255, 255, 255);
				}
				while(x!=x_end);
			}

			if(line1_draw_y==true && line3_draw_y==true)
			{
				loop_nop=false;
				//std::cout<< "5" << std::endl;
				line1_draw_y=false;
				line3_draw_y=false;

				int32_t x,y,x_end,y_end;
				if(line1_fastx==true)
				{
					x=line1_s;
					y=line1_t;
				}
				else
				{
					x=line1_t;
					y=line1_s;
				}
				if(line3_fastx==true)
				{
					x_end=line3_s;
					y_end=line3_t;
				}
				else
				{
					x_end=line3_t;
					y_end=line3_s;
				}
				int32_t x_step=0;
				if(x<x_end)
				{
					x_step=1;
				}
				if(x>x_end)
				{
					x_step=-1;
				}
				do
				{
					x+=x_step;
					draw_pixel(x, y, 255, 255, 255);
				}
				while(x!=x_end);
			}

			//end loop
			if((line1_s==line1_s_end && line2_s==line2_s_end && line3_s==line3_s_end))
			{
				//std::cout<< "6" << std::endl;
				loop_quit=true;
			}

				if(loop_nop==true)
				{
					loop_quit=true;
				}
				loop_nop=true;
				//std::cout<< "7" << std::endl;
				//SDL_Delay(5000);

		}

	}
}