#include "EntityBase.h"


namespace base
{
	const size_t EntityBase::get_next_entity_type_id()
	{
		return next_entity_type_id++;
	}

	EntityBase::EntityBase(const size_t tid) : my_type_id(tid)
	{
	}


	EntityBase::~EntityBase()
	{
	}

	bool EntityBase::remove_renderpack(const render::RenderData & pack)
	{
		const size_t size = my_render_data.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (&my_render_data[i] == &pack)
			{
				my_render_data.erase(my_render_data.begin() + i);
				pack.~RenderData();
				return true;
			}
		}
		return false;
	}

	void EntityBase::remove_all_renderdata()
	{
		// probably not needed , will be called by "clear"
		//const size_t size = render_data.size();
		//for (size_t i = 0; i < size; ++i)
		//{
		//	render_data[i].~RenderData();
		//}
		my_render_data.clear();
	}

	render::RenderData &EntityBase::add_renderpack()
	{
		my_render_data.insert( my_render_data.end(),render::RenderData() );
		return my_render_data.back();
	}

	void EntityBase::for_all_renderpacks(std::function<void(render::RenderData&pack)> func)
	{
		const size_t size = my_render_data.size();
		for (size_t i = 0; i < size; ++i)
		{
			func(my_render_data[i]);
		}
	}

	size_t EntityBase::get_type_id() const
	{
		return my_type_id;
	}

	void EntityBase::get_renderdata(const std::vector<render::RenderData> * &data) const
	{
		data = (&my_render_data); 
	}

	render::RenderData &EntityBase::get_renderpack(const size_t index, render::RenderData *&data)
	{
		data = &my_render_data.at(index);
		return *data;
	}

	const render::RenderData &EntityBase::get_renderpack(const size_t index, const render::RenderData *&data) const 
	{
		data = &my_render_data.at(index);
		return *data;
	}


	size_t EntityBase::next_entity_type_id = 1;

}