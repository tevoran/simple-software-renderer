#include <glm/glm.hpp>
#define LOADBMP_IMPLEMENTATION
#include <loadbmp.h>
#define FAST_OBJ_IMPLEMENTATION
#include <fast_obj.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>
#include <string>

#include "ssr.hpp"

#include <ctime>

#define scroll_speed 1.0f
#define reps 1

#define reps2 100000000

int main(int argc, char **argv)
{
	try
	{
		if(argc!=3)
		{
			std::cout << "[WARNING] not the right arguments given" << std::endl;
			return 0;
		}


		ssr::texture texture(argv[2]); //second argument is the path to the texture


		//SDL_Delay(100000);
		//initializing renderer
		ssr::renderer renderer(1920, 1080 ,0.5*PI, (float)16/(float)9, 0.01, 1000000);
		uint32_t num_polygons=0;
		glm::vec3 min_coords=glm::vec3(0,0,0);
		glm::vec3 max_coords=glm::vec3(0,0,0);
		ssr::vertex *mesh_vertices=renderer.load_mesh(argv[1], &num_polygons, &max_coords, &min_coords);

		//automatic positioning the mesh
		float distance=4;
		glm::vec3 mesh_pos=glm::vec3(0,0,0);
		mesh_pos.x=-(min_coords.x+max_coords.x)/2;
		mesh_pos.y=-(min_coords.y+max_coords.y)/2;
		mesh_pos.z=sqrt(
							(min_coords.x*min_coords.x)+
							(min_coords.y*min_coords.y)+
							(min_coords.z*min_coords.z)+
							(max_coords.x*max_coords.x)+
							(max_coords.y*max_coords.y)+
							(max_coords.z*max_coords.z))*distance;

		glm::vec3 rot_axis=glm::vec3(0,1,0);

		//FPS counter initialization
		int FPS_count=0;
		std::clock_t clock_begin = std::clock();
		std::clock_t clock_end = std::clock();
		float frametime_s=0;
		std::clock_t frame_begin =std::clock();
		std::clock_t frame_end = std::clock();

		SDL_Event ev;
		for(float i=0; i<1000 ; i=i+0.005)
		{
			//FPS counter
			std::clock_t clock_end = std::clock();
			frame_end=std::clock();
			frametime_s=((float)frame_end-(float)frame_begin)/(float)CLOCKS_PER_SEC;
			frame_begin=frame_end;

			if((clock_end-clock_begin)*1000/CLOCKS_PER_SEC>1000)
			{
				std::cout << 
							FPS_count << 
							" FPS (average frametime: " << 
							((clock_end-clock_begin)*1000/CLOCKS_PER_SEC)/FPS_count << 
							"ms)" << 
							std::endl;
				clock_begin=clock_end;
				FPS_count=0;
			}
			FPS_count++;

			//handling mouse and keyboard input
			while(SDL_PollEvent(&ev))
			{
				if(ev.type==SDL_MOUSEWHEEL)
				{
					//scrolling up
					if(ev.wheel.y>0)
					{
						distance=distance+(0.1*(distance/2)*(distance/2)+frametime_s*scroll_speed);
					}
					//scrolling down
					if(ev.wheel.y<0)
					{
						distance=distance-(0.1*(distance/4)*(distance/4)+frametime_s*scroll_speed);
					}
					if(distance<0.7)
					{
						distance=0.7;
					}
					if(distance>100)
					{
						distance=100;
					}
				}
			}
			renderer.validate_window(); //the window needs to be validated again because of some SDL2 internal stuff

			mesh_pos.z=sqrt(
							(min_coords.x*min_coords.x)+
							(min_coords.y*min_coords.y)+
							(min_coords.z*min_coords.z)+
							(max_coords.x*max_coords.x)+
							(max_coords.y*max_coords.y)+
							(max_coords.z*max_coords.z))*distance;

			//rendering mesh
			renderer.render(mesh_vertices, num_polygons, mesh_pos, &rot_axis, i, &texture, SSR_FILL);
			renderer.update();
		}

	}
	catch(const char *msg)
	{
		std::cout << msg << std::endl;
	}



	//SDL_Delay(5000);
	return 0;
}
