#pragma once
#include "WorldBase.h"

class WorldRenderer
{
public:
	WorldRenderer(WorldBase &world);
	virtual ~WorldRenderer();
	/**
	updates the output 
	@param deltaTime
		time passed in seconds
	**/
	virtual void step(const float deltaTime);
};

