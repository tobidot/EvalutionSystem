#include "EntityInfoLine.h"
#include <consoleapi.h>


EntityInfoLine::EntityInfoLine() : base::EntityBase(TYPE_ID)
{
	my_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	render::RenderData &render = add_renderpack();
	render.set_position(0, 0.9f, 1.0f);
	render.draw_type = render::Draw_Type::TEXT;
	render::RenderParameterText *text = render.get_package<render::RenderParameterText>();
	text->text = new char[80];
	text->text[0] = 0;
}


EntityInfoLine::~EntityInfoLine()
{
}


void EntityInfoLine::update(const float deltaTime)
{
	CONSOLE_SCREEN_BUFFER_INFO *screen_info = new CONSOLE_SCREEN_BUFFER_INFO();
	GetConsoleScreenBufferInfo(my_console_handle, screen_info);

	render::RenderData *render = nullptr;
	get_renderpack(0, render);

	render::RenderParameterText *text = render->get_package<render::RenderParameterText>();
	sprintf_s(text->text, 80, "%d , %d  max %d , %d", screen_info->dwSize.X, screen_info->dwSize.Y, screen_info->dwMaximumWindowSize.X, screen_info->dwMaximumWindowSize.Y);
	sprintf_s(text->text, 80, "window  l %d , r %d , t %d , b %d  ", screen_info->srWindow.Left, screen_info->srWindow.Right, screen_info->srWindow.Top, screen_info->srWindow.Bottom);

	//SetConsoleTitle(text->text);

	delete screen_info;
}

void EntityInfoLine::get_renderdata(const std::vector<render::RenderData>*  &data) const
{
	return base::EntityBase::get_renderdata(data);
}

const size_t EntityInfoLine::TYPE_ID = base::EntityBase::get_next_entity_type_id();