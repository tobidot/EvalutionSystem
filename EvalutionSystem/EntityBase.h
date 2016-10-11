#pragma once
#include <vector>
#include <functional>
#include "RenderData.h"

namespace base
{
	class EntityBase
	{
	private:
		static size_t next_entity_type_id;
	protected:
		static const size_t get_next_entity_type_id();
	private:
		const size_t my_type_id;
		std::vector<render::RenderData> my_render_data;
	public:
		EntityBase(const size_t tid);
		virtual ~EntityBase();
		bool remove_renderpack(const render::RenderData &pack);
		void remove_all_renderdata();
		render::RenderData &add_renderpack();
		void for_all_renderpacks(std::function<void(render::RenderData &pack)> func);
		size_t get_type_id() const;

		virtual void update(const float deltaTime) = 0;
		virtual void get_renderdata(const std::vector<render::RenderData>* &data) const;
		virtual render::RenderData &get_renderpack(const size_t index, render::RenderData* &data);
		virtual const render::RenderData &get_renderpack(const size_t index,const render::RenderData* &data) const;
		
	};
}

