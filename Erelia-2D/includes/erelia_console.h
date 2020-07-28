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

	jgl::Text_entry* _entry;
	jgl::Array<jgl::String> _old_entry;

public:
	Console(Board* p_board, Player* p_player, jgl::Widget* p_parent = nullptr);
	jgl::Text_entry* entry() { return (_entry); }

	bool handle_console_entry(jgl::String cmd);

	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif