#include "erelia.h"

Console::Console(Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_board = p_board;
	_player = p_player;

	_entry = new jgl::Text_entry("", this);
	_entry->activate();
}

bool Console::handle_console_entry(jgl::String cmd)
{
	jgl::Array<jgl::String> tab = cmd.split(" ");

	if (tab[0] == "save")
	{

	}
	else if (tab[0] == "load")
	{

	}
	else if (tab[0] == "tp")
	{
		if (tab.size() == 3)
		{
			jgl::Vector2 dest = jgl::Vector2(jgl::stof(tab[1]), jgl::stof(tab[2]));
			_old_entry.push_back("Teleport to " + dest.str());
			_player->place(dest);
		}
		else
			_old_entry.push_back("Error in TP command");
		return (true);
	}
	else if (tab[0] == "speed")
	{
		if (tab.size() == 2)
		{
			float speed = jgl::stof(tab[1]);
			_old_entry.push_back("Setting speed to " + jgl::ftoa(speed));
			_player->set_move_speed(speed);
		}
		else
			_old_entry.push_back("Error in speed modifier command");
		return (true);
	}
	return (false);
}

bool Console::handle_keyboard()
{
	if (jgl::get_key(jgl::key::return_key) == jgl::key_state::release)
	{
		if (handle_console_entry(_entry->text()) == false)
			_old_entry.push_back(_entry->text());
		_entry->set_text("");
		_entry->select();
	}
	return (false);
}

bool Console::handle_mouse()
{
	return (false);
}

void Console::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_entry->set_geometry(jgl::Vector2(5.0f, p_area.y - 35), jgl::Vector2(p_area.x * 0.75f, 30.0f));
}

void Console::render()
{
	for (size_t i = 0; i < _old_entry.size(); i++)
	{
		jgl::Vector2 tmp = _entry->anchor() - jgl::Vector2(0.0f, 30.0f);
		jgl::Vector2 pos = tmp + jgl::Vector2(20.0f, i * -21.0f);
		jgl::draw_text(_old_entry[_old_entry.size() - i - 1], pos, 16, 0, jgl::text_color::white);
	}
}