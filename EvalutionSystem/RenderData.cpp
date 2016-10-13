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
		this->position = ScreenRectangle(x,y,x+width,y);
	}

}