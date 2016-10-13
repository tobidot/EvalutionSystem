#include <stdlib.h>
#include "Timer.h"
#include "WorldBase.h"
#include "WorldEvalution.h"
#include "WorldRenderEvalution.h"
#include "EntityTest.h"
#include "EntityInfoLine.h"
#include <iostream>
#include <io.h>


// i can easily cofigure my world here
void initialize(WorldEvalution &world)
{	
	for (int i = 0; i < 10; ++i)
	{
		world.entity_create(new EntityTest((rand()%50)/50.0f, (rand() % 50) / 50.0f));
	}
	world.entity_create(new EntityInfoLine());
	
}

// shutdown the world
void shutdown(WorldEvalution &world)
{

}

int main(int argc,char* argv[])
{
	Timer timer;
	WorldEvalution world;
	WorldRenderEvalution renderer(&world);
	initialize(world);
	do
	{
		float timePassed = timer.get();
		timer.reset();
		world.step(timePassed);
		renderer.step(timePassed);
	} while (world.is_running());
	shutdown(world);

	std::cout << "ciao" << std::endl;
	char buff[255];
	std::cin.getline(buff, 255);
	return 0;
}