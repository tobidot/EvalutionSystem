#pragma once
#include "WorldRenderer.h"


class WorldRenderEvalution : public render::WorldRenderer
{
private:
	HANDLE my_console_input_handle;
	HANDLE my_console_output_handle;
public:
	WorldRenderEvalution(base::WorldBase *const world);
	virtual ~WorldRenderEvalution();
	virtual void initialize_console();


	// Geerbt über WorldRenderer
	virtual void render_entity(base::EntityBase *const entity, const float deltaTime) override;
	virtual void step(const float deltaTime) override;

	virtual void render_data(render::RenderData &pack, const float deltaTime);
	virtual void render_text_data(render::RenderData &pack, const float deltaTime);

};

