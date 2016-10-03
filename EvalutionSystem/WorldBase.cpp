#include "WorldBase.h"



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

void WorldBase::stop()
{
	running = true;
}

void WorldBase::pause()
{
	pause = true;
}

void WorldBase::play()
{
	pause = false;
}

bool WorldBase::is_pausing() const
{
	return pause;
}

bool WorldBase::is_running()
{
	return running;
}

void WorldBase::step(const float deltaTime)
{
	update_map(deltaTime);
	update_entities(deltaTime);
}

void WorldBase::update_entities(const float deltaTime)
{
	
}

void WorldBase::update_map(const float deltaTime)
{
}
