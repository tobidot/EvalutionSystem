#include "EntityBase.h"



EntityBase::EntityBase()
{
}


EntityBase::~EntityBase()
{
}

bool EntityBase::remove_renderpack(const render::RenderData & pack)
{
	const size_t size = render_data.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (&render_data[i] == &pack)
		{
			render_data.erase(render_data.begin()+i);
			pack.~RenderData();
			return true;
		}
	}
	return false;
}

void EntityBase::remove_all_renderdata()
{
	const size_t size = render_data.size();
	for (size_t i = 0; i < size; ++i)
	{
		render_data[i].~RenderData();
	}
	render_data.clear();
}
