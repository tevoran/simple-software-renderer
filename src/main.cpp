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
		glm::vec3 mesh_pos=glm::vec3(0,0,0);
		mesh_pos.x=-(min_coords.x+max_coords.x)/2;
		mesh_pos.y=-(min_coords.y+max_coords.y)/2;
		mesh_pos.z=sqrt(
							(min_coords.x*min_coords.x)+
							(min_coords.y*min_coords.y)+
							(min_coords.z*min_coords.z)+
							(max_coords.x*max_coords.x)+
							(max_coords.y*max_coords.y)+
							(max_coords.z*max_coords.z))*4;

		glm::vec3 rot_axis=glm::vec3(0,1,0);


		for(float i=0.0; i<100; i=i+0.003)
		{
				//mesh_pos.z+=0.01;
				//std::cout << mesh_pos.z << std::endl;
			std::clock_t clock_begin = std::clock();	
			renderer.render(mesh_vertices, num_polygons, mesh_pos, &rot_axis, i, &texture, SSR_FILL);
			renderer.update();
			std::clock_t clock_end = std::clock();
			std::cout << "frame time: " << (clock_end-clock_begin)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
			//std::cout << "repetitions: " << reps << std::endl;
			//SDL_Delay(10000);
			//exit(0);
		}

	}
	catch(const char *msg)
	{
		std::cout << msg << std::endl;
	}



	//SDL_Delay(5000);
	return 0;
}
