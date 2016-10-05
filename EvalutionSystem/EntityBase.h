#pragma once
#include <vector>
#include "RenderData.h"

class EntityBase
{
private:
	std::vector<render::RenderData> render_data;
public:
	EntityBase();
	virtual ~EntityBase();
	bool remove_renderpack(const render::RenderData &pack);
	void remove_all_renderdata();


	virtual void update(const float deltaTime) = 0;
	virtual void get_renderdata(std::vector<render::RenderData> &data) const = 0;

};

