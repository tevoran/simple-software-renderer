#include "ssr.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdint>
#include <iostream>
#include <math.h>
#include <exception>
#include <algorithm>

ssr::renderer::triangle_line_rendering::triangle_line_rendering(glm::ivec2 in_start, glm::ivec2 in_end)
{
	//implementing necessary variables for the bresenham algorithm
	start=in_start;
	end=in_end;

	d=glm::ivec2(end.x-start.x, end.y-start.y);
	
	if(d.x*d.x>=d.y*d.y)
	{
		fast_x=true;
	}
	else
	{
		fast_x=false;
	}

	if(fast_x==true)
	{
		s=start.x;
		s_end=end.x;

		t=start.y;
		t_end=end.y;
	}
	else
	{
		s=start.y;
		s_end=end.y;

		t=start.x;
		t_end=end.x;
	}

	if(s<s_end)
		{
			s_step=1;
		}
		else if(s>s_end)
		{
			s_step=-1;
		}
		else
		{
			s_step=0;
		}

	if(t<t_end)
		{
			t_step=1;
		}
		else if(t>t_end)
		{
			t_step=-1;
		}
		else
		{
			t_step=0;
		}

	ds=s_end-s;
	dt=t_end-t;
	error=ds/2;
}

void ssr::renderer::triangle_line_rendering::triangle_line_iterate()
{
	if(s==s_end || line_finished==true)
	{
		line_finished=true;
		y_update_ready=true;
		return;
	}

	s+=s_step;
	if(dt>=0)
	{
		error-=dt;
	}
	else
	{
		error+=dt;
	}

	y_update_ready=false;
	if(fast_x==false)
	{
		y_update_ready=true;
	}

	if(error<=0)
	{
		t+=t_step;
		if(ds>=0)
		{
			error+=ds;
		}
		else
		{
			error-=ds;
		}

		if(fast_x==true)
		{
			y_update_ready=true;
		}
	}
}

uint32_t ssr::renderer::triangle_line_rendering::get_x()
{
	uint32_t out=fast_x*s+(!fast_x*t);
	return out;
}

void ssr::renderer::triangle_line_rendering::show_variables()
{
	std::cout << "start: X x Y: " << start.x << " x " << start.y << std::endl;
	std::cout << "end: X x Y: " << end.x << " x " << end.y << std::endl;
	std::cout << "d: X x Y: " << d.x << " x " << d.y << std::endl;
	std::cout << "t: " << t << std::endl;
	std::cout << "t_end: " << t_end << std::endl;
	std::cout << "t_step: " << t_step << std::endl;
	std::cout << "dt: " << dt << std::endl;
	std::cout << "s: " << s << std::endl;
	std::cout << "s_end: " << s_end << std::endl;
	std::cout << "s_step: " << s_step << std::endl;
	std::cout << "ds: " << ds << std::endl;
	std::cout << "fast_x: " << fast_x << std::endl;
	std::cout << "error: " << error << std::endl;
	std::cout << "y_update_ready: " << y_update_ready << std::endl;
	std::cout << "line_finished: " << line_finished << std::endl;
}

bool ssr::renderer::triangle_line_rendering::line_done()
{
	return line_finished;
}

bool ssr::renderer::triangle_line_rendering::y_update()
{
	return y_update_ready;
}

void ssr::renderer::triangle_line_rendering::y_update_processed()
{
	y_update_ready=false;
}