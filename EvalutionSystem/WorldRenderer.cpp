#include "WorldRenderer.h"
#include <vector>


WorldRenderer::WorldRenderer(WorldBase &world) : world(world)
{
}


WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::step(const float deltaTime)
{
	render_entities(deltaTime);
}

void WorldRenderer::render_entities(const float deltaTime)
{
	world.for_all_entities( [this, deltaTime](EntityBase &entity)-> bool { render_entity(entity, deltaTime); return true; } );
}

