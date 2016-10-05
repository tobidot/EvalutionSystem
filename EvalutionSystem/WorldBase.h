#pragma once
#include <functional>
#include <vector>
#include "EntityBase.h"

class WorldBase
{
private :
	std::vector<EntityBase> entities;
protected:
	bool running;
	bool pausing;
public:
	WorldBase();
	virtual ~WorldBase();
	void stop();
	void pause();
	void play();
	bool is_pausing() const;
	bool is_running() const;
	void for_all_entities(std::function<bool(EntityBase&)> func);
	// virtuals

	/*
	updates the world 
	@param deltaTime
		time passed in Seconds
	*/
	virtual void step(const float deltaTime);
	virtual bool entity_kill(const EntityBase &entity);
	virtual EntityBase &entity_create(EntityBase *entity = nullptr);
	

private :
protected:
	void update_entities(const float deltaTime);
	void update_map(const float deltaTime);
	void update_entity(EntityBase &entity,const float deltaTime) = 0;
	/*
	do not use if possible
	*/
	const std::vector<EntityBase> get_entities() const;
};

