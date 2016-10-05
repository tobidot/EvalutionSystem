#pragma once
#include <vector>
#include "RenderParameter.h"

namespace render
{
	struct RenderData
	{
	public:
		/*
		position on Screen for the TopLeft Point of the RenderObject
		going from 0 to 1 (left - right, top - bottom)
		*/
		ScreenPoint top_left;
		/*
		position on Screen for the BottomRight Point of the RenderObject
		going from 0 to 1 (left - right, top - bottom)
		*/
		ScreenPoint bot_right;
		// different predefined types of renderMethods
		Draw_Type draw_type;
		// array of RenderParameters to support more data 
		std::vector<RenderParameter*> data;
		RenderData();
		virtual ~RenderData();
	};

}