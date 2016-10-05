#include "WorldBase.h"
#include "UsedExceptions.h"


WorldBase::WorldBase()
{
	running = false;
}


WorldBase::~WorldBase()
{
}

bool WorldBase::is_running() const
{
	return false;
}

const std::vector<EntityBase> WorldBase::get_entities() const
{
	return std::vector<EntityBase>();
}

void WorldBase::stop()
{
	running = true;
}

void WorldBase::pause()
{
	pausing = true;
}

void WorldBase::play()
{
	pausing = false;
}

bool WorldBase::is_pausing() const
{
	return pause;
}

void WorldBase::for_all_entities(std::function<bool(EntityBase&)> func)
{
	const size_t size = entities.size();
	for (size_t i = 0; i< size; ++i)
	{
		if (&entities[i] != nullptr)
		{
			if ( !func(entities[i]) ) return;
		}
	}
}

bool WorldBase::is_running() const
{
	return running;
}

void WorldBase::step(const float deltaTime)
{
	if (pausing == false)
	{
		update_map(deltaTime);
		update_entities(deltaTime);
	}
}

bool WorldBase::entity_kill(const EntityBase & entity)
{
	const size_t size = entities.size();
	for (size_t i = 0; i< size; ++i)
	{
		if (&entities[i] != &entity)
		{
			// TODO erase later at a safe point
			entities.erase(entities.begin() + i);
			return true;
		}
	}
	return false;
}

EntityBase & WorldBase::entity_create(EntityBase *entity)
{
	if (entity == nullptr)
	{
		// TODO
		// automaticly create an entity 
		//entity = 
		throw UsedExceptions::NotImplementedYet();
	}
	// insert entity at the end of the vector
	entities.insert(entities.end(),*entity);
	return *entity;
}

void WorldBase::update_entities(const float deltaTime)
{
	const size_t size = entities.size();
	for(size_t i = 0;i< size; ++i)
	{
		if ( &entities[i] != nullptr ) update_entity( entities[i], deltaTime);
	}
}

void WorldBase::update_map(const float deltaTime)
{
	// TODO update map-tiles
}

