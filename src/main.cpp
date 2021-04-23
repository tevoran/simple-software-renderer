#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

#include "ssr.hpp"

#include <ctime>
#define reps 1

#define reps2 100000000

int main()
{
	try
	{
		//initializing renderer
		ssr::renderer renderer(1920, 1080 ,0.5*PI, (float)16/(float)9, 0.01, 1000);

		ssr::vertex vertices[9];
		
		//second test triangle
		ssr::vertex vertex4={1,	0,	9,	200,200,2,	1, 1};// yellow
		ssr::vertex vertex5={1,	0,	7,	200,200,2,	0, 1};
		ssr::vertex vertex6={0,	1,	3,	200,200,2,	1, 0};

		vertices[3]=vertex4;
		vertices[4]=vertex5;
		vertices[5]=vertex6;

		ssr::vertex vertex7={4,	0,	10,	100,100,2,	0,	0};// yellow
		ssr::vertex vertex8={5,	0,	10,	100,100,2,	1,	0};
		ssr::vertex vertex9={2,	1,	10,	100,100,2,	1,	1};

		vertices[6]=vertex7;
		vertices[7]=vertex8;
		vertices[8]=vertex9;

		//test texture
		ssr::texture texture;
		texture.x=4;
		texture.y=4;
		texture.pixel_data=new uint32_t[texture.x*texture.y];
		texture.pixel_data[0]=0x00500050;
		texture.pixel_data[1]=0x00151515;
		texture.pixel_data[2]=0x00500050;
		texture.pixel_data[3]=0x00151515;
		
		texture.pixel_data[4]=0x00151515;
		texture.pixel_data[5]=0x00500050;
		texture.pixel_data[6]=0x00151515;
		texture.pixel_data[7]=0x00500050;

		texture.pixel_data[8]=0x00500050;
		texture.pixel_data[9]=0x00151515;
		texture.pixel_data[10]=0x00500050;
		texture.pixel_data[11]=0x00151515;

		texture.pixel_data[12]=0x00151515;
		texture.pixel_data[13]=0x00500050;
		texture.pixel_data[14]=0x00151515;
		texture.pixel_data[15]=0x00500050;


		for(float i=0.1; i<100; i=i+0.01)
		{
			
							ssr::vertex vertex1={-1,	-1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	0, 0}; //dark green
							ssr::vertex vertex2={1,		-1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	0, 1};
							ssr::vertex vertex3={-1,	1+5*cos(i)+3,	10*sin(i)+15,	20,100,0,	1, 0};

							vertices[0]=vertex1;
							vertices[1]=vertex2;
							vertices[2]=vertex3;

							std::clock_t clock_begin = std::clock();	
							for(uint32_t i = reps; i > 0; i--)
							{
								renderer.render(vertices, 3, texture, SSR_FILL);
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
