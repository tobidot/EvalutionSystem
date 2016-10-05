#pragma once
#include "WorldBase.h"

class WorldRenderer
{
public :
private:
	WorldBase &world;
public:
	WorldRenderer(WorldBase &world);
	virtual ~WorldRenderer();
	/**
	updates the output 
	@param deltaTime
		time passed in seconds
	**/
	virtual void step(const float deltaTime);

private:

protected:
	virtual void render_entities(const float deltaTime);
	virtual void render_entity(EntityBase &entity, const float deltaTime) = 0;
};

