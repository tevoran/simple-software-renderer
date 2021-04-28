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
		ssr::renderer renderer(1920, 1080 ,0.5*PI, (float)16/(float)9, 0.01, 10000);
		uint32_t num_polygons=0;
		ssr::vertex *mesh_vertices=renderer.load_mesh(argv[1], &num_polygons);
		glm::vec3 mesh_pos=glm::vec3(0,500,4000);
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
