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
		ssr::renderer renderer;

		//test triangle
		ssr::vertex vertex1={0.1,	0.2,	0.1,	20,100,0};
		ssr::vertex vertex2={0.8,	0.2,	0.5,	20,100,0};
		ssr::vertex vertex3={0.5,	0.8,	0.9,	20,100,0};

		//second test triangle
		ssr::vertex vertex4={0.0,	0.0,	0.8,	200,200,2};
		ssr::vertex vertex5={0.7,	1,		0.01,	200,200,2};
		ssr::vertex vertex6={1.1,	0.1,	0.8,	200,200,2};

		for(float i=1.6*PI; i<100; i=i+1)
		{
					//vertex2={0.3*cos(i)+0.5, 0.3*(float)sin(i)+0.5, 1, 255, 255, 255};
							std::cout << "rendering:" << std::endl;
							std::clock_t clock_begin = std::clock();	
							for(uint32_t i = reps; i > 0; i--)
							{
								renderer.render(vertex1, vertex2, vertex3, SSR_FILL);
								renderer.render(vertex4, vertex5, vertex6, SSR_FILL);
							}
							std::clock_t clock_end = std::clock();
							std::cout << "execution time: " << (clock_end-clock_begin)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
							std::cout << "repetitions: " << reps << std::endl;

							renderer.update();
							SDL_Delay(3000);
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
