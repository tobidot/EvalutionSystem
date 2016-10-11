#include "WorldRenderer.h"
#include <vector>

namespace render
{

	WorldRenderer::WorldRenderer(base::WorldBase *const world) : world(world)
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
		world->for_all_entities([this, deltaTime](base::EntityBase *const entity)-> bool { render_entity(entity, deltaTime); return true; });
	}

}