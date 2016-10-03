#include <stdlib.h>
#include "Timer.h"
#include "WorldBase.h"
#include "WorldRenderer.h"

// i can easily cofigure my world here
void initialize(WorldBase &world)
{

}

// shutdown the world
void shutdown(WorldBase &world)
{

}

int main(int argc,char* argv[])
{
	Timer timer;
	WorldBase world;
	WorldRenderer renderer(world);
	initialize(world);
	do
	{
		float timePassed = timer.get();
		timer.reset();
		world.step(timePassed);
		renderer.step(timePassed);
	} while (world.is_running());
	shutdown(world);
	return 0;
}