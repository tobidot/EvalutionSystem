#include "WorldEvalution.h"



WorldEvalution::WorldEvalution()
{

}


WorldEvalution::~WorldEvalution()
{
}

void WorldEvalution::update_entity(base::EntityBase *const entity, const float deltaTime)
{
	if (entity != nullptr)
	{
		entity->update(deltaTime);
	}
}
