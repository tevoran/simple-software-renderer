#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

#include "ssr.hpp"

#include <ctime>
#define reps 1

int main()
{
	try
	{
		//initializing renderer
		ssr::renderer renderer;

		//test triangle
		ssr::vertex vertex1={0.5,0.5,1,255,255,255};
		ssr::vertex vertex2={0.5,0.4,1,255,255,255};
		ssr::vertex vertex3={0.9,0.5,1,255,255,255};


		for(float i=0; i<10; i=i+0.001)
		{
					vertex2={0.2*cos(i)+0.5, 0.2*(float)sin(i)+0.5, 1, 255, 255, 255};

							std::clock_t clock_begin = std::clock();
							for(uint32_t i = reps; i > 0; i--)
							{
								renderer.render(vertex1, vertex2, vertex3);	
							}
							std::clock_t clock_end = std::clock();
							std::cout << "execution time: " << (clock_end-clock_begin)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
							std::cout << "repetitions: " << reps << std::endl;

					renderer.clear(0,0,0);
		}

	}
	catch(const char *msg)
	{
		std::cout << msg << std::endl;
	}



	//SDL_Delay(5000);
	return 0;
}