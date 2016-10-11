#include "RenderData.h"

namespace render
{ 

	RenderData::RenderData()
	{
	}


	RenderData::~RenderData()
	{
		const size_t size = data.size();
		for (size_t i= 0; i<size; ++i)
		{
			RenderParameter *param = data[i];
			delete param;
		}
		data.clear();
	}

	void RenderData::set_position(float x, float y, float width)
	{
		this->top_left.x = x;
		this->top_left.y = y;
		this->bot_right.x = x + width;
		this->bot_right.y = y;
	}

}