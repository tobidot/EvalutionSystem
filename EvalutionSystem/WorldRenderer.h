#pragma once
#include "WorldBase.h"

namespace render
{
	class WorldRenderer
	{
	public:
	private:
		base::WorldBase *const world;
	public:
		WorldRenderer(base::WorldBase *const world);
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
		virtual void render_entity(base::EntityBase *const entity, const float deltaTime) = 0;
	};

}