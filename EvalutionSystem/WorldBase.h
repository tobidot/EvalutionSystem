#pragma once
#include <vector>

class EntityBase;
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
	/*
	do not use if possible
	*/
	const std::vector<EntityBase> get_entities() const;

	// virtuals

	/*
	updates the world 
	@param deltaTime
		time passed in Seconds
	*/
	virtual void step(const float deltaTime);
	virtual void entity_kill(EntityBase &entity);
	virtual EntityBase &entity_create(EntityBase *entity = nullptr);

private :
protected:
	void update_entities(const float deltaTime);
	void update_map(const float deltaTime);
	void update_entity(EntityBase &entity,const float deltaTime);
};

