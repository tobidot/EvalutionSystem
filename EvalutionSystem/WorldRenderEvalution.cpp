#include "WorldRenderEvalution.h"
#include "UsedExceptions.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <iostream>
#include <conio.h>
#include <consoleapi.h>
#include <iomanip>
#include <stdint.h>

COLORREF COLORREF_of(uint8_t r, uint8_t g, uint8_t b)
{
	return b + (g << 8) + (r << 16);
}

WorldRenderEvalution::WorldRenderEvalution(base::WorldBase *const world) : render::WorldRenderer(world)
{
	my_console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	my_console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
	initialize_console();
}

WorldRenderEvalution::~WorldRenderEvalution()
{
}

void WorldRenderEvalution::initialize_console()
{
	COLORREF color_table[16] = {
		// dark colors
		COLORREF_of(0,0,0),
		COLORREF_of(127,0,0),
		COLORREF_of(0,127,0),
		COLORREF_of(0,0,127),
		COLORREF_of(127,127,0),
		COLORREF_of(127,0,127),
		COLORREF_of(0,127,127),
		COLORREF_of(127,127,127),
		// light colors
		COLORREF_of(63,63,63),
		COLORREF_of(255,0,0),
		COLORREF_of(0,255,0),
		COLORREF_of(0,0,255),
		COLORREF_of(255,255,0),
		COLORREF_of(255,0,255),
		COLORREF_of(0,255,255),
		COLORREF_of(255,255,255),
	};
	// make Cursor invisible
	CONSOLE_CURSOR_INFO cursor_info = CONSOLE_CURSOR_INFO();
	cursor_info.bVisible = false;
	cursor_info.dwSize = 3;


	// set Console Info
	CONSOLE_SCREEN_BUFFER_INFOEX screen_info = CONSOLE_SCREEN_BUFFER_INFOEX();
	screen_info.bFullscreenSupported = true;
	screen_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	for (size_t i = 0; i < 16; ++i) screen_info.ColorTable[i] = color_table[i];
	screen_info.dwCursorPosition.X = 0;
	screen_info.dwCursorPosition.Y = 0;
	screen_info.dwMaximumWindowSize.X = 80;
	screen_info.dwMaximumWindowSize.Y = 30;
	screen_info.dwSize.X = 80;
	screen_info.dwSize.Y = 30;
	screen_info.srWindow.Left = 0;
	screen_info.srWindow.Right = 30;
	screen_info.srWindow.Top = 0;
	screen_info.srWindow.Bottom = 30;
	screen_info.wAttributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	screen_info.wPopupAttributes = BACKGROUND_RED | FOREGROUND_BLUE;

	// initialize EventMode
	DWORD console_mode = ENABLE_WINDOW_INPUT;

	if (!SetConsoleMode(my_console_input_handle, console_mode))
	{
		throw UsedExceptions::ExtendedException("Unable to set console mode.");
		return;
	};
	if (!SetConsoleCursorInfo(my_console_output_handle, &cursor_info) )
	{
		throw UsedExceptions::ExtendedException("Unable to set cursor.");
		return;
	};
	if (!SetConsoleScreenBufferInfoEx(my_console_output_handle, &screen_info) )
	{
		throw UsedExceptions::ExtendedException("Unable to set consolebuffer.");
		return;
	};
}

void WorldRenderEvalution::render_entity(base::EntityBase *const entity, const float deltaTime)
{
	entity->for_all_renderpacks([this,deltaTime](render::RenderData &pack) {render_data(pack,deltaTime); });
}

int cEvent = 0;

void WorldRenderEvalution::step(const float deltaTime)
{
	INPUT_RECORD *input_list = new INPUT_RECORD[16];
	DWORD count = 0;
		PeekConsoleInput(my_console_input_handle, input_list, 16, &count);
	if (count > 0)
	{
		ReadConsoleInput(my_console_input_handle, input_list, 16, &count);
	}
	for (DWORD i = 0; i < count; ++i)
	{
		INPUT_RECORD &input = input_list[i];
		if (input.EventType == WINDOW_BUFFER_SIZE_EVENT)
		{
			WINDOW_BUFFER_SIZE_RECORD &event = input.Event.WindowBufferSizeEvent;
		}
	}
	delete input_list;
	++cEvent;
	if (cEvent % 10000 == 0)
	{
		char str[80];
		sprintf_s(str, 80, "number %d", cEvent);
		SetConsoleTitle(str);

		SMALL_RECT rect = { 0,0,50,30 };
		//SetConsoleWindowInfo(my_console_input_handle, true, &rect);
		SetConsoleWindowInfo(my_console_output_handle, true, &rect);
		COORD size = { 50,30 };
		//SetConsoleScreenBufferSize(my_console_input_handle, size);
		SetConsoleScreenBufferSize(my_console_output_handle, size);
	}

	WorldRenderer::step(deltaTime);
}

void WorldRenderEvalution::render_data(render::RenderData & pack, const float deltaTime)
{
	switch (pack.draw_type)
	{
	case render::Draw_Type::DRAW:
		throw UsedExceptions::NotImplementedYet();
		break;
	case render::Draw_Type::EXTRA:
		throw UsedExceptions::NotImplementedYet();
		break;
	case render::Draw_Type::FILL:
		throw UsedExceptions::NotImplementedYet();
		break;
	case render::Draw_Type::IMAGE:
		throw UsedExceptions::NotImplementedYet();
		break;
	case render::Draw_Type::TEXT:
		render_text_data(pack,deltaTime);
		break;
	default:
		break;
	}
}

void WorldRenderEvalution::render_text_data(render::RenderData & pack, const float deltaTime)
{
	CONSOLE_SCREEN_BUFFER_INFO *screen_info = new CONSOLE_SCREEN_BUFFER_INFO();
	GetConsoleScreenBufferInfo(my_console_output_handle, screen_info);
	SHORT console_height = 80;
	SHORT console_width = 30;

	SHORT row = (int)((pack.bot_right.y + pack.top_left.y) * console_height * 0.5f);
	SHORT start_collum = (int)(pack.top_left.x * console_width);
	SHORT collums_to_write = (int)(pack.bot_right.x * console_width) - start_collum;

	// Ich weis, dass ich Text zeichne
	char *text = nullptr;
	render::Color text_color;
	render::Color text_bg_color;
	// fill needed parameters with data 
	for (render::RenderParameter *param : pack.data)
	{
		if (param->type_id == render::RenderParameterColor::TYPE_ID)
		{
			text_color = static_cast<render::RenderParameterColor*>(param)->forground;
			text_bg_color = static_cast<render::RenderParameterColor*>(param)->background;
		}
		else if (param->type_id == render::RenderParameterText::TYPE_ID)
		{
			text = static_cast<render::RenderParameterText*>(param)->text;	
		}
		else
		{
			throw UsedExceptions::NotImplementedYet();
		}
	}

	if (text != nullptr)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD position = { start_collum, row };

		SetConsoleCursorPosition(hStdout, position);
		std::cout << std::setfill(' ') << std::setw(collums_to_write) << text;
	}
	else
	{
		// TODO insufficient Parameter exception
	}


}
