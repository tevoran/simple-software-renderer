#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

#include "ssr.hpp"

#include <ctime>
#define reps 1000

#define reps2 100000000

int main()
{
	try
	{
		//initializing renderer
		ssr::renderer renderer(0.5*PI, (float)16/(float)16, 0.01, 1000);

		ssr::vertex vertices[9];
		//test triangle
		ssr::vertex vertex1={0.0,	-0.2,	0.11,	20,100,0}; //dark green
		ssr::vertex vertex2={0.8,	-0.2,	0.11,	20,100,0};
		ssr::vertex vertex3={0.4,	0.8,	0.11,	20,100,0};

		vertices[0]=vertex1;
		vertices[1]=vertex2;
		vertices[2]=vertex3;

		//second test triangle
		ssr::vertex vertex4={-0.5,	0.0,	0.12,	200,200,2};// yellow
		ssr::vertex vertex5={-0.7,	-1,		0.13,	200,200,2};
		ssr::vertex vertex6={1.1,	0.1,	100,	200,200,2};

		vertices[3]=vertex4;
		vertices[4]=vertex5;
		vertices[5]=vertex6;

		ssr::vertex vertex7={4,	0,	10,	100,100,2};// yellow
		ssr::vertex vertex8={5,	0,	10,	100,100,2};
		ssr::vertex vertex9={2,	1,	10,	100,100,2};

		vertices[6]=vertex7;
		vertices[7]=vertex8;
		vertices[8]=vertex9;

		for(float i=0.1; i<100; i=i+0.01)
		{
			std::cout << "z: " << i << std::endl;
							ssr::vertex vertex1={0,	0,	i,	20,100,0}; //dark green
							ssr::vertex vertex2={1,	0,	i,	20,100,0};
							ssr::vertex vertex3={0,	1,	i,	20,100,0};

							vertices[0]=vertex1;
							vertices[1]=vertex2;
							vertices[2]=vertex3;

							std::clock_t clock_begin = std::clock();	
							for(uint32_t i = reps; i > 0; i--)
							{
								renderer.render(vertices, 3, SSR_FILL);
								renderer.update();
							}
							std::clock_t clock_end = std::clock();
							std::cout << "execution time: " << (clock_end-clock_begin)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
							std::cout << "repetitions: " << reps << std::endl;
							exit(0);
		}

	}
	catch(const char *msg)
	{
		std::cout << msg << std::endl;
	}



	//SDL_Delay(5000);
	return 0;
}
