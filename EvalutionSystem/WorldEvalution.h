#pragma once
#include "WorldBase.h"
class WorldEvalution :
	public base::WorldBase
{
public:
	WorldEvalution();
	virtual ~WorldEvalution();


	// Geerbt �ber WorldBase
	virtual void update_entity(base::EntityBase *const entity, const float deltaTime) override;

};

