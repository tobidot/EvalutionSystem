#include "EntityTest.h"



EntityTest::EntityTest(float x,float y) : base::EntityBase(TYPE_ID)
{
	counter = 0;
	render::RenderData &render = this->add_renderpack();
	render.draw_type = render::Draw_Type::TEXT;
	render.position = render::ScreenRectangle(x, y, x + 0.01f, y);
	render::RenderParameterText *text = new render::RenderParameterText();
	render.data.insert(render.data.end(), text);
	text->text = new char[10];
	//sprintf_s(text->text, "%3d", 0u);
	text->text[0] = 'H';
	text->text[1] = 0;
	render::RenderParameterColor *color = new render::RenderParameterColor();
	render.data.insert(render.data.end(), color);
	color->forground = render::Color(1, 1, 1);
	color->forground= render::Color(0, 0, 0);
}


EntityTest::~EntityTest()
{
}

void EntityTest::update(const float deltaTime)
{
	++counter;
	const std::vector<render::RenderData> *render_data;
	get_renderdata(render_data);
	for (const render::RenderData &render_pack : *render_data )
	{
		const render::RenderParameterText *package_text = render_pack.get_package<render::RenderParameterText>();
		if (package_text)
		{
			package_text->text[0] = '0' + counter % 10;
		}
	}
}

void EntityTest::get_renderdata(const std::vector<render::RenderData>*  &data) const
{
	return base::EntityBase::get_renderdata(data);
}

const size_t EntityTest::TYPE_ID = base::EntityBase::get_next_entity_type_id();