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
#include <assert.h>

COLORREF COLORREF_of(uint8_t r, uint8_t g, uint8_t b)
{
	return (b << 16) + (g << 8) + r;
}

WorldRenderEvalution::WorldRenderEvalution(base::WorldBase *const world) : render::WorldRenderer(world)
{
	// get handles
	my_console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	my_console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
	my_consol_window_handle = GetConsoleWindow();
	my_consol_menu = GetSystemMenu(my_consol_window_handle, false);
	assert(my_console_input_handle && my_console_output_handle && my_consol_menu && my_consol_window_handle );


	// screen
	my_screen_is_updated = true;
	my_screen_update_timer = 0.0f;
	my_screen_size = { 100,45 };
	my_screen_rect = { 0,0,my_screen_size.X-1,my_screen_size.Y - 1 };
	my_screen_buffer = new CHAR_INFO[my_screen_size.X * my_screen_size.Y];
	assert(my_screen_buffer);
	for (WORD i = 0; i < my_screen_size.X * my_screen_size.Y; ++i)
	{
		my_screen_buffer[i].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		my_screen_buffer[i].Char.AsciiChar = 'X';
	}
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
		COLORREF_of(0,0,127),
		COLORREF_of(0,127,0),
		COLORREF_of(0,127,127),
		COLORREF_of(127,0,0),
		COLORREF_of(127,0,127),
		COLORREF_of(127,127,0),
		COLORREF_of(127,127,127),
		// light colors
		COLORREF_of(63,63,63),
		COLORREF_of(0,0,255),
		COLORREF_of(0,255,0),
		COLORREF_of(0,255,255),
		COLORREF_of(255,0,0),
		COLORREF_of(255,0,255),
		COLORREF_of(255,255,0),
		COLORREF_of(255,255,255),
	};


	// initialize EventMode
	DWORD console_mode = ENABLE_MOUSE_INPUT;

	
	DWORD err = GetLastError(); // just to debug
	if (!SetConsoleMode(my_console_input_handle, console_mode))
	{
		err = GetLastError();
		throw UsedExceptions::ExtendedException("Unable to set console mode.");
		return;
	};


	// make Cursor invisible
	CONSOLE_CURSOR_INFO cursor_info = CONSOLE_CURSOR_INFO();
	cursor_info.bVisible = false;
	cursor_info.dwSize = 3;
	if (!SetConsoleCursorInfo(my_console_output_handle, &cursor_info) )
	{
		err = GetLastError();
		throw UsedExceptions::ExtendedException("Unable to set cursor.");
		return;
	};


	// set Console Info
	CONSOLE_SCREEN_BUFFER_INFOEX screen_info = CONSOLE_SCREEN_BUFFER_INFOEX();
	screen_info.bFullscreenSupported = false;
	screen_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	for (size_t i = 0; i < 16; ++i) screen_info.ColorTable[i] = color_table[i];
	screen_info.dwCursorPosition.X = 0;
	screen_info.dwCursorPosition.Y = 0;
	screen_info.dwMaximumWindowSize.X = my_screen_size.X;	// needs to be one larger than the actual window (or not ... im confused)
	screen_info.dwMaximumWindowSize.Y = my_screen_size.Y;
	screen_info.dwSize.X = my_screen_size.X;			// characters in a row
	screen_info.dwSize.Y = my_screen_size.Y;			// rows in the console
	screen_info.srWindow.Left = 0;		// inklusive (show the character at this position)
	screen_info.srWindow.Top = 0;
	screen_info.srWindow.Right = my_screen_size.X-1;	// inklusiv (will show the last) 
	screen_info.srWindow.Bottom = my_screen_size.Y-1;	// so its the same as the amount of chars -1
	screen_info.wAttributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	screen_info.wPopupAttributes = BACKGROUND_RED | FOREGROUND_BLUE;
	if (!SetConsoleScreenBufferInfoEx(my_console_output_handle, &screen_info) )
	{
		err = GetLastError();
		throw UsedExceptions::ExtendedException("Unable to set consolebuffer.");
		return;
	};


	// Resize the window again
	// since it seems like ScreenBufferinfo can't 
	// generate a bigger window than previos info.dwMaximumWindowSize

	if (!SetConsoleWindowInfo(my_console_output_handle, true, &my_screen_rect) )
	{
		err = GetLastError();
		throw UsedExceptions::ExtendedException("Unable to set windowsize.");
		return;
	}
	/*  //debugging
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(my_console_output_handle, &info);*/



	// delete some menu-options to prevent the user from resizing
	//DeleteMenu(sysMenu, SC_CLOSE, MF_BYCOMMAND);
	DeleteMenu(my_consol_menu, SC_MINIMIZE, MF_BYCOMMAND);
	DeleteMenu(my_consol_menu, SC_MAXIMIZE, MF_BYCOMMAND);
	DeleteMenu(my_consol_menu, SC_SIZE, MF_BYCOMMAND);
}

void WorldRenderEvalution::render_entity(base::EntityBase *const entity, const float deltaTime)
{
	entity->for_all_renderpacks([this,deltaTime](render::RenderData &pack) {render_data(pack,deltaTime); });
}

void WorldRenderEvalution::step(const float deltaTime)
{
	process_console_inputs(deltaTime);
	// do a update for all renderObjects
	WorldRenderer::step(deltaTime);
	my_screen_update_timer += deltaTime;
	if ( my_screen_is_updated && my_screen_update_timer >= 0.2f)
	{
		my_screen_update_timer = 0.0f;
		WriteConsoleOutput(my_console_output_handle, my_screen_buffer, my_screen_size, { 0,0 }, &my_screen_rect);
		my_screen_is_updated = false;
	}
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
	SHORT console_width = my_screen_size.X;
	SHORT console_height = my_screen_size.Y;

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
		for (WORD i = 0; i < collums_to_write; ++i)
		{
			// TODO 
			// safety check
			// and updatecheck
			if (text[i] == 0) break;
			WORD index = start_collum * my_screen_size.X + i + row;
			if (index < my_screen_size.X * my_screen_size.Y)
			{
				if (my_screen_buffer[index].Char.AsciiChar != text[i])
				{
					my_screen_buffer[index].Char.AsciiChar = text[i];
					my_screen_is_updated = true;
				}
			}
		}
	}
	else
	{
		// TODO insufficient Parameter exception
	}


}

void WorldRenderEvalution::process_console_inputs(const float deltaTime)
{
	// input management 
	DWORD events_on_stack = 0;
	DWORD read_events = 0;
	INPUT_RECORD *input_list = nullptr;
	GetNumberOfConsoleInputEvents(my_console_input_handle, &events_on_stack);
	if (events_on_stack > 0)
	{
		input_list = new INPUT_RECORD[events_on_stack];
		ReadConsoleInput(my_console_input_handle, input_list, events_on_stack, &read_events);
		for (DWORD i = 0; i < read_events; ++i)
		{
			INPUT_RECORD &input = input_list[i];
			switch (input.EventType)
			{
				case WINDOW_BUFFER_SIZE_EVENT:
				{
					WINDOW_BUFFER_SIZE_RECORD &event = input.Event.WindowBufferSizeEvent;
				}
				break;
				case MOUSE_EVENT:
				{
					MOUSE_EVENT_RECORD &event = input.Event.MouseEvent;
				}
				break;
				case MENU_EVENT:
				{
					MENU_EVENT_RECORD &event = input.Event.MenuEvent;
				}
				break;
				case KEY_EVENT:
				{
					KEY_EVENT_RECORD &event = input.Event.KeyEvent;
				}
				break;	
				case FOCUS_EVENT:
				{
					FOCUS_EVENT_RECORD &event = input.Event.FocusEvent;
				}
				break;
			}
		}
		delete input_list;
	}
}
