#ifndef ERELIA_CONSOLE_H
#define ERELIA_CONSOLE_H

#include "jgl.h"
#include "erelia_board.h"
#include "erelia_player.h"

class Console_printer : public jgl::Widget
{
private:
	jgl::Array<jgl::String> _old_entry;
	jgl::Array<Uint32> _entry_time;

public:
	Console_printer(jgl::Widget* parent = nullptr) : jgl::Widget(parent)
	{
		_old_entry.clear();
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{

	}

	void add_entry(jgl::String text)
	{
		_old_entry.push_front(text);
		_entry_time.push_front(g_time);
	}

	void render();
};

class Console : public jgl::Widget
{
private:
	bool _complete;

	Console_printer* _printer;

	jgl::Text_entry* _entry;
	size_t _cmd_index;
	jgl::Array<jgl::String> _old_cmd;

public:
	Console(jgl::Widget* p_engine = nullptr);
	jgl::Text_entry* entry() { return (_entry); }

	jgl::Array<size_t> parse_regex_simple(jgl::String text);
	jgl::Array<size_t> parse_regex_percent(jgl::String text);

	void enable();
	void disable();

	bool complete() { return (_complete); }

	bool handle_console_entry(jgl::String cmd);

	bool handle_clear_command(jgl::Array<jgl::String>& tab);
	bool handle_warp_command(jgl::Array<jgl::String>& tab);
	bool handle_save_command(jgl::Array<jgl::String>& tab);
	bool handle_load_command(jgl::Array<jgl::String>& tab);
	bool handle_tp_command(jgl::Array<jgl::String>& tab);
	bool handle_speed_command(jgl::Array<jgl::String>& tab);
	bool handle_generate_command(jgl::Array<jgl::String>& tab);
	bool handle_replace_command(jgl::Array<jgl::String>& tab);
	bool handle_prefab_command(jgl::Array<jgl::String>& tab);
	bool handle_ghost_command(jgl::Array<jgl::String>& tab);
	bool handle_coord_command(jgl::Array<jgl::String>& tab);
	bool handle_link_command(jgl::Array<jgl::String>& tab);
	bool handle_npc_command(jgl::Array<jgl::String>& tab);
	bool handle_area_command(jgl::Array<jgl::String>& tab);

	bool handle_unknow_command(jgl::String cmd);

	bool return_funct(jgl::String text, bool state);

	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif