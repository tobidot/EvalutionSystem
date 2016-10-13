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

WORD TEXT_ATTRIBUTES_of(const render::Color &color)
{
	WORD attrib = 0;
	float heightest_intesity = max(max(max(color.r,color.g),color.b),0.33f);
	if (color.r > heightest_intesity * 0.5f)
	{
		attrib |= FOREGROUND_RED;
	}
	if (color.g > heightest_intesity * 0.5f)
	{
		attrib |= FOREGROUND_GREEN;
	}
	if (color.b > heightest_intesity * 0.5f)
	{
		attrib |= FOREGROUND_BLUE;
	}
	if (heightest_intesity > 0.66f)
	{
		attrib |= FOREGROUND_INTENSITY;
	}
	return attrib;
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
	my_screen_needs_update_complete = true;
	my_screen_update_rect_count = 0;
	my_screen_update_timer = 0.0f;
	my_screen_size = { 100,45 };
	my_screen_rect = { 0,0,my_screen_size.X-1,my_screen_size.Y - 1 };
	my_screen_buffer = new CHAR_INFO[my_screen_size.X * my_screen_size.Y];
	assert(my_screen_buffer);
	for (WORD i = 0; i < my_screen_size.X * my_screen_size.Y; ++i)
	{
		my_screen_buffer[i].Attributes =  FOREGROUND_INTENSITY | BACKGROUND_BLUE |BACKGROUND_GREEN |BACKGROUND_RED;
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

void WorldRenderEvalution::fill_screen_rect(const SMALL_RECT & target, const char * text, WORD textAttributes)
{
	size_t text_len = strlen(text);
	SHORT start_x = (target.Left+target.Right)/2-text_len;
	SHORT start_y = (target.Top + target.Bottom)/2;
	SHORT width = (target.Right - target.Left)+1;
	SHORT height = (target.Bottom - target.Top) +1;
	if (start_x < target.Left) start_x = target.Left;

	bool changed = false;
	for (WORD j= 0; j < height; ++j)
	{
		for (WORD i = 0; i < width; ++i)
		{
			if (text[j*width + i] == 0)
			{	// by reaching end of text end the loop
				j = height;
				break;
			}
			const SHORT x = start_x + i;
			const SHORT y = start_y + j;
			WORD index = y * my_screen_size.X + x;
			if ( index < my_screen_size.X * my_screen_size.Y)
			{	// i can now update the Pixel
				if (my_screen_buffer[index].Char.AsciiChar != text[i] || my_screen_buffer[index].Attributes != textAttributes)
				{	// screen actually changed
					my_screen_buffer[index].Char.AsciiChar = text[i];
					my_screen_buffer[index].Attributes = textAttributes;
					changed = true;
				}
			}
		}
	}
	// keep track of the change
	if (changed)
	{
		if (my_screen_update_rect_count < MAX_UPDATABLE_RECTANGLES)
		{	// store the updated part of the screen
			my_screen_updating_rects[my_screen_update_rect_count] = target;
			++my_screen_update_rect_count;
		}
		else
		{	// maximum of updating rects was used -> screen needs full update
			my_screen_needs_update_complete = true;
		}
	}
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
	if ( my_screen_needs_update_complete && my_screen_update_timer >= 0.2f)
	{
		my_screen_update_timer = 0.0f;
		WriteConsoleOutput(my_console_output_handle, my_screen_buffer, my_screen_size, { 0,0 }, &my_screen_rect);
		my_screen_needs_update_complete = false;
	}
	else if (my_screen_update_rect_count > 0)
	{
		for (SHORT i = 0; i < my_screen_update_rect_count; ++i)
		{
			WriteConsoleOutput(my_console_output_handle,
				my_screen_buffer, my_screen_size, {my_screen_updating_rects[i].Left,my_screen_updating_rects[i].Top}, &my_screen_updating_rects[i]);
		}
	}
	my_screen_update_rect_count = 0;
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
	// Ich weis, dass ich Text zeichne
	char *text = nullptr;
	WORD attributes = DEFAULT_TEXT_ATTRIBUTES;
	// fill needed parameters with data 
	for (render::RenderParameter *param : pack.data)
	{
		if (param->type_id == render::RenderParameterColor::TYPE_ID)
		{
			render::Color text_color;
			render::Color text_bg_color;
			text_color = static_cast<render::RenderParameterColor*>(param)->forground;
			text_bg_color = static_cast<render::RenderParameterColor*>(param)->background;
			// convert color to attributes
			attributes = TEXT_ATTRIBUTES_of(text_color) | (TEXT_ATTRIBUTES_of(text_bg_color) * BACKGROUND_BLUE);
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

	SHORT console_width = my_screen_size.X;
	SHORT console_height = my_screen_size.Y;
	const render::ScreenPoint &center = pack.position.get_center();

	SMALL_RECT position = {
		(SHORT)(pack.position.left_top.x * console_width),
		(SHORT)(center.y * console_height),
		(SHORT)(pack.position.bottom_right.x * console_width),
		(SHORT)(center.y * console_height) };
	if (text != nullptr)
	{
		fill_screen_rect(position, text, attributes);
	}
	else
	{
		// TODO insufficient Parameter exception
		throw "insufficient parameter";
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
