#pragma once
#include "EntityBase.h"
class EntityTest :
	public base::EntityBase
{
public:
	static const size_t TYPE_ID;
private:
	int counter;
public:
	EntityTest(float x = 0,float y = 0);
	~EntityTest();

	// Geerbt über EntityBase
	virtual void update(const float deltaTime) override;
	virtual void get_renderdata(const std::vector<render::RenderData>* &data) const override;
};

