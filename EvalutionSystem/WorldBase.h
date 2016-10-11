#pragma once
#include <functional>
#include <vector>
#include "EntityBase.h"

namespace base
{
	class WorldBase
	{
	private:
		std::vector<EntityBase*> entities;
	protected:
		bool my_running;
		bool my_pause;
	public:
		WorldBase();
		virtual ~WorldBase();
		void stop();
		void pause();
		void play();
		bool is_pausing() const;
		bool is_running() const;
		void for_all_entities(std::function<bool(EntityBase*const)> func) const;
		// virtuals

		/*
		updates the world
		@param deltaTime
			time passed in Seconds
		*/
		virtual void step(const float deltaTime);
		virtual bool entity_kill(const EntityBase *const entity);
		virtual EntityBase *const entity_create(EntityBase *const entity = nullptr);


	private:
	protected:
		void update_entities(const float deltaTime);
		void update_map(const float deltaTime);
		virtual void update_entity(EntityBase *const entity, const float deltaTime) = 0;
		/*
		do not use if possible
		*/
		const std::vector<EntityBase*> &get_entities() const;
	};

}