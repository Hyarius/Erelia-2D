#ifndef ERELIA_CONSOLE_H
#define ERELIA_CONSOLE_H

#include "jgl.h"
#include "erelia_board.h"
#include "erelia_player.h"

class Console : public jgl::Widget
{
private:
	Board* _board;
	Player* _player;
	jgl::Sprite_sheet* _tileset;
	class Editor_interact* _interacter;
	class Editor_inventory* _inventory;

	jgl::Text_entry* _entry;
	jgl::Array<jgl::String> _old_entry;

public:
	Console(Editor_inventory* p_inventory, jgl::Sprite_sheet* p_tileset, Editor_interact* p_interacter, Board* p_board, Player* p_player, jgl::Widget* p_parent = nullptr);
	jgl::Text_entry* entry() { return (_entry); }

	jgl::Array<size_t> parse_regex_simple(jgl::String text);
	jgl::Array<size_t> parse_regex_percent(jgl::String text);

	bool handle_console_entry(jgl::String cmd);

	bool handle_save_command(jgl::Array<jgl::String>& tab);
	bool handle_load_command(jgl::Array<jgl::String>& tab);
	bool handle_tp_command(jgl::Array<jgl::String>& tab);
	bool handle_speed_command(jgl::Array<jgl::String>& tab);
	bool handle_generate_command(jgl::Array<jgl::String>& tab);
	bool handle_replace_command(jgl::Array<jgl::String>& tab);
	bool handle_prefab_command(jgl::Array<jgl::String>& tab);
	bool handle_ghost_command(jgl::Array<jgl::String>& tab);
	bool handle_coord_command(jgl::Array<jgl::String>& tab);

	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif