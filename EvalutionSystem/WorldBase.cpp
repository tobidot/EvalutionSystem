#include "WorldBase.h"
#include "UsedExceptions.h"


namespace base
{
	WorldBase::WorldBase()
	{
		my_running = true;
		my_pause = false;
	}

	WorldBase::~WorldBase()
	{
		for (EntityBase *entity : entities)
		{
			delete entity;
		}
		entities.clear();
	}

	bool WorldBase::is_running() const
	{
		return my_running;
	}

	const std::vector<EntityBase*> &WorldBase::get_entities() const
	{
		return entities;
	}

	void WorldBase::stop()
	{
		my_running = true;
	}

	void WorldBase::pause()
	{
		my_pause = true;
	}

	void WorldBase::play()
	{
		my_pause = false;
	}

	bool WorldBase::is_pausing() const
	{
		return my_pause;
	}

	void WorldBase::for_all_entities(std::function<bool(EntityBase*const )> func) const
	{
		const size_t size = entities.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (&entities[i] != nullptr)
			{
				if (!func(entities[i])) return;
			}
		}
	}

	void WorldBase::step(const float deltaTime)
	{
		if (my_pause == false)
		{
			update_map(deltaTime);
			update_entities(deltaTime);
		}
	}

	bool WorldBase::entity_kill(const EntityBase *entity)
	{
		const size_t size = entities.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (entities[i] != entity)
			{
				// TODO erase later at a safe point
				entities.erase(entities.begin() + i);
				return true;
			}
		}
		return false;
	}

	EntityBase *const WorldBase::entity_create(EntityBase *const entity)
	{
		if (entity == nullptr)
		{
			// TODO
			// automaticly create an entity 
			//entity = 
			throw UsedExceptions::NotImplementedYet();
		}
		// insert entity at the end of the vector
		entities.insert(entities.end(), entity);
		return entity;
	}

	void WorldBase::update_entities(const float deltaTime)
	{
		const size_t size = entities.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (&entities[i] != nullptr) update_entity(entities[i], deltaTime);
		}
	}

	void WorldBase::update_map(const float deltaTime)
	{
		// TODO update map-tiles
	}

}