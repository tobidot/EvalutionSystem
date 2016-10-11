#pragma once
#include <consoleapi.h>
#include "EntityBase.h"

class EntityInfoLine :
	public base::EntityBase
{
public:
	static const size_t TYPE_ID;
private:
	HANDLE my_console_handle;
public:
	EntityInfoLine();
	~EntityInfoLine();


	virtual void update(const float deltaTime) override;
	virtual void get_renderdata(const std::vector<render::RenderData>* &data) const override;
};

