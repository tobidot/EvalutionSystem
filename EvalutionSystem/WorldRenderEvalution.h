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

	bool my_screen_needs_update_complete;
	float my_screen_update_timer;
	// up to 10 rectangles that can be updatet without refreshing the whole screen
	SHORT my_screen_update_rect_count;
	SMALL_RECT my_screen_updating_rects[10];
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

