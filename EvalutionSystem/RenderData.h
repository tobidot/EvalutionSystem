#pragma once
#include <vector>
#include "RenderParameter.h"

namespace render
{
	struct RenderData
	{
	public:
		/*
		position on Screen for the BottomRight Point of the RenderObject
		going from 0 to 1 (left - right, top - bottom)
		*/
		ScreenRectangle position;
		// different predefined types of renderMethods
		Draw_Type draw_type;
		// array of RenderParameters to support more data 
		std::vector<RenderParameter*> data;
		RenderData();
		virtual ~RenderData();

		void set_position(float x, float y, float width);
		template<class TYPE> TYPE *const get_package() const;
		template<class TYPE> TYPE *const get_package();

	};

	template<class TYPE>
	TYPE * const RenderData::get_package() const
	{
		for (RenderParameter *param : data)
		{
			if (param->type_id == TYPE::TYPE_ID)
			{
				return static_cast<TYPE*>(param);
			}
		}
		// no package found -> create one
		return nullptr;
	}

	template<class TYPE>
	TYPE * const RenderData::get_package()
	{
		for (RenderParameter *param : data)
		{
			if (param->type_id == TYPE::TYPE_ID)
			{
				return static_cast<TYPE*>(param);
			}
		}
		// no package found -> create one
		TYPE *param = new TYPE();
		this->data.insert( this->data.end(), param);
		return param;
	}

}