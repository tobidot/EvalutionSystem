#pragma once
#include "WorldRenderer.h"


class WorldRenderEvalution : public render::WorldRenderer
{

private:
private:
	// consol and window handles
	HANDLE my_console_input_handle = nullptr;
	HANDLE my_console_output_handle = nullptr;
	HMENU my_consol_menu = nullptr;
	HWND my_consol_window_handle = nullptr;
	// console definitions
	COORD my_screen_size;
	SMALL_RECT my_screen_rect;
	CHAR_INFO *my_screen_buffer = nullptr;
	bool my_screen_is_updated;
public:
	WorldRenderEvalution(base::WorldBase *const world);
	virtual ~WorldRenderEvalution();
	virtual void initialize_console();


	// Geerbt über WorldRenderer
	virtual void render_entity(base::EntityBase *const entity, const float deltaTime) override;
	virtual void step(const float deltaTime) override;

	virtual void render_data(render::RenderData &pack, const float deltaTime);
	virtual void render_text_data(render::RenderData &pack, const float deltaTime);
	virtual void process_console_inputs(const float deltaTime);
private:

};

