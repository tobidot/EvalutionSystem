#pragma once
#include <consoleapi.h>

namespace render
{

	struct ScreenPoint
	{
		float x, y;
		ScreenPoint(float x = 0, float y = 0) { this->x = x; this->y = y; };
		ScreenPoint(const ScreenPoint &sc) { this->x = sc.x; this->y = sc.y; };
	};
	struct ScreenRectangle
	{
		ScreenPoint left_top;
		ScreenPoint bottom_right;
		ScreenRectangle(float left = 0, float top = 0, float right = 0, float bottom = 0) { this->left_top = ScreenPoint(left,top); this->bottom_right = ScreenPoint(right,bottom ); };
		ScreenRectangle(const ScreenPoint &leto, const ScreenPoint &bore) { this->left_top = leto; this->bottom_right = bore; };
		ScreenRectangle(const ScreenRectangle &sr) { this->left_top = sr.left_top; this->bottom_right = sr.bottom_right; };
		ScreenPoint get_center() { return ScreenPoint((left_top.x + bottom_right.x)*0.5f, (left_top.y + bottom_right.y)*0.5f); };
	};
	enum Draw_Type
	{
		// Draw an image (resource ID in data)
		IMAGE,
		TEXT,
		DRAW,
		FILL,
		EXTRA
	};
	struct Color
	{
		float r, g, b, a;
		Color(float r = 1.0f, float g = 1.0f, float  b = 1.0f, float  a = 1.0f) : r(r), g(g), b(b), a(a) {};
	};

	struct RenderParameter
	{
	protected:
		static size_t NEXT_RENDER_PARAMETER_ID;
		static size_t get_next_render_parameter_id() { return NEXT_RENDER_PARAMETER_ID++; };
	public:
	public:
		size_t type_id = 0;
		RenderParameter(const size_t id) : type_id(id) {};
	};

	struct RenderParameterID : RenderParameter
	{
		static const size_t TYPE_ID;
		char *id_sequence = nullptr;
		RenderParameterID() : RenderParameter(TYPE_ID) {};
	};

	struct RenderParameterColor : RenderParameter
	{
		static const size_t TYPE_ID;
		Color forground;
		Color background;
		RenderParameterColor() : RenderParameter(TYPE_ID) {};
	};

	struct RenderParameterText : RenderParameter
	{
		static const size_t TYPE_ID;
		char *text;
		RenderParameterText() : RenderParameter(TYPE_ID) {};
	};
}
