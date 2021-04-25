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
			return 0;
		}

		fastObjMesh *teapot = fast_obj_read(argv[1]); //first argument is the path to the mesh
		ssr::texture texture(argv[2]); //second argument is the path to the texture

		//initializing renderer
		ssr::renderer renderer(1920, 1080 ,0.5*PI, (float)16/(float)9, 0.01, 1000);


		ssr::vertex vertices[12];
		
		//second test triangle
		ssr::vertex vertex4={-5,	1,		10,		200,200,2,	1, 1};// yellow
		ssr::vertex vertex5={-3,	0,		2,		200,200,2,	0, 1};
		ssr::vertex vertex6={-2,	-1,		30,		200,200,2,	1, 0};

		vertices[3]=vertex4;
		vertices[4]=vertex5;
		vertices[5]=vertex6;

		ssr::vertex vertex7={4,	0,	10,	100,100,2,	0,	0};// yellow
		ssr::vertex vertex8={5,	0,	10,	100,100,2,	1,	0};
		ssr::vertex vertex9={2,	1,	10,	100,100,2,	1,	1};

		vertices[6]=vertex7;
		vertices[7]=vertex8;
		vertices[8]=vertex9;



		for(float i=0.1; i<100; i=i+0.01)
		{
			
							ssr::vertex vertex1={-1,	-1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	0, 0}; //dark green
							ssr::vertex vertex2={1,		-1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	1, 0};
							ssr::vertex vertex3={-1,	1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	0, 1};

							vertices[0]=vertex1;
							vertices[1]=vertex2;
							vertices[2]=vertex3;

							ssr::vertex vertex10={-1,	1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	0, 1}; //dark green
							ssr::vertex vertex11={1,	1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	1, 1};
							ssr::vertex vertex12={1,	-1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	1, 0};

							vertices[9]=vertex10;
							vertices[10]=vertex11;
							vertices[11]=vertex12;

							std::clock_t clock_begin = std::clock();	
							for(uint32_t i = reps; i > 0; i--)
							{
								renderer.render(vertices, 4, &texture, SSR_FILL);
								renderer.update();
							}
							std::clock_t clock_end = std::clock();
							std::cout << "execution time: " << (clock_end-clock_begin)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
							std::cout << "repetitions: " << reps << std::endl;
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
