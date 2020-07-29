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
		if (tab.size() == 2)
		{
			_board->save("ressources/maps/" + tab[1] + ".map");
		}
		else
			_old_entry.push_back("Usage : save [map path]");
		return (true);
	}
	else if (tab[0] == "load")
	{
		if (tab.size() == 2)
		{
			_board->load("ressources/maps/" + tab[1] + ".map");
		}
		else
			_old_entry.push_back("Usage : load [map path]");
		return (true);
	}
	else if (tab[0] == "tp")
	{
		if (tab.size() == 3)
		{
			jgl::Vector2 dest = jgl::Vector2(jgl::stof(tab[1]), jgl::stof(tab[2]));
			_player->place(dest);
			_old_entry.push_back("Teleport to " + _player->pos().str());
		}
		else
			_old_entry.push_back("Usage : tp [coord X][coord Y]");
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
			_old_entry.push_back("Usage : speed [speed value]");
		return (true);
	}
	else if (tab[0] == "square")
	{
		if (tab.size() == 4)
		{
			jgl::Vector2 size = jgl::Vector2(jgl::stof(tab[1]), jgl::stof(tab[2]));
			jgl::Vector2 start = _player->pos() - size / 2;
			jgl::Vector2 end = _player->pos() + size / 2;
			jgl::Array<size_t> elem;
			jgl::Array<jgl::String> regex = tab[3].split(",");
			for (size_t i = 0; i < regex.size(); i++)
			{
				jgl::Array<jgl::String> regex_part = regex[i].split("%");
				if (regex_part.size() != 2)
				{
					_old_entry.push_back("Usage : square [square size X] [square size Y] [bloc regex]");
					_old_entry.push_back("Regex exemple : 1%50,2%12,3%13,4%25");
					return (true);
				}
				int index = jgl::stoi(regex_part[0]);
				int nb = jgl::stoi(regex_part[1]);
				if (index < 0 || index >= node_array.size())
				{
					_old_entry.push_back("Usage : square [square size X] [square size Y] [bloc regex]");
					_old_entry.push_back("Error in regex : Bad index");
					return (true);
				}
				else if (nb <= 0)
				{
					_old_entry.push_back("Usage : square [square size X] [square size Y] [bloc regex]");
					_old_entry.push_back("Error in regex : Bad probability value");
					return (true);
				}

				for (size_t j = 0; j < nb; j++)
					elem.push_back(index);
			}

			for (float i = start.x; i <= end.x; i++)
				for (float j = start.y; j <= end.y; j++)
				{
					size_t tmp = elem[jgl::generate_nbr(0, elem.size())];
					_board->place(jgl::Vector2(i, j), tmp, false);
				}
			start = (start / chunk_size).floor();
			end = (end / chunk_size).floor();
			for (float i = start.x; i <= end.x; i++)
				for (float j = start.y; j <= end.y; j++)
				{
					_board->bake_chunk(jgl::Vector2(i, j));
				}
			jgl::String result_string = "Square of " + size.str() + " size created with node ";
			for (size_t i = 0; i < regex.size(); i++)
			{
				jgl::Array<jgl::String> regex_part = regex[i].split("%");
				if (i != 0)
				{
					result_string += " - ";
				}
				result_string += regex_part[0];
			}
			_old_entry.push_back(result_string);
		}
		else
		{
			_old_entry.push_back("Usage : square [square size X] [square size Y] [bloc regex]");
			_old_entry.push_back("Regex exemple : 1%50,2%12,3%13,4%25");
		}
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