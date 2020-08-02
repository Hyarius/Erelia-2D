#include "erelia.h"

Console::Console(Editor_inventory* p_inventory, jgl::Sprite_sheet* p_tileset, Editor_interact* p_interacter, Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_tileset = p_tileset;
	_board = p_board;
	_player = p_player;
	_inventory = p_inventory;
	_interacter = p_interacter;

	_entry = new jgl::Text_entry("", this);
	_entry->activate();
}

bool Console::handle_save_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		_board->save("ressources/maps/" + tab[1] + ".map");
	}
	else
		_old_entry.push_back("Usage : save [map path]");
	return (true);
}
bool Console::handle_load_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		_board->load("ressources/maps/" + tab[1] + ".map");
	}
	else
		_old_entry.push_back("Usage : load [map path]");
	return (true);
}
bool Console::handle_tp_command(jgl::Array<jgl::String>& tab)
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
bool Console::handle_speed_command(jgl::Array<jgl::String>& tab)
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

jgl::Array<size_t> Console::parse_regex_percent(jgl::String text)
{
	jgl::Array<size_t> elem;
	jgl::Array<jgl::String> regex = text.split(",");
	for (size_t i = 0; i < regex.size(); i++)
	{
		jgl::Array<jgl::String> regex_part = regex[i].split("%");
		if (regex_part.size() != 2)
		{
			_old_entry.push_back("Error : Token number error in regex");
			_old_entry.push_back("Usage : generate [bloc regex]");
			_old_entry.push_back("Regex exemple : 1%50,2%12,3%13,4%25");
			return (jgl::Array<size_t>());
		}
		int index = jgl::stoi(regex_part[0]);
		int nb = jgl::stoi(regex_part[1]);
		if (index < 0 || index >= static_cast<int>(node_array.size()))
		{
			_old_entry.push_back("Error : node index out of range");
			_old_entry.push_back("Usage : generate [bloc regex]");
			_old_entry.push_back("Error in regex : Bad index");
			return (jgl::Array<size_t>());
		}
		else if (nb <= 0)
		{
			_old_entry.push_back("Error : Percent negative");
			_old_entry.push_back("Usage : generate [bloc regex]");
			_old_entry.push_back("Error in regex : Bad probability value");
			return (jgl::Array<size_t>());
		}

		for (int j = 0; j < nb; j++)
			elem.push_back(index);
	}
	return (elem);
}

jgl::Array<size_t> Console::parse_regex_simple(jgl::String text)
{
	jgl::Array<size_t> elem;
	jgl::Array<jgl::String> regex = text.split(",");
	for (size_t i = 0; i < regex.size(); i++)
	{

		int index = jgl::stoi(regex[i]);
		if (index < 0 || index >= static_cast<int>(node_array.size()))
		{
			_old_entry.push_back("Error : node index out of range");
			_old_entry.push_back("Usage : generate [bloc regex]");
			_old_entry.push_back("Error in regex : Bad index");
			return (jgl::Array<size_t>());
		}
		elem.push_back(index);
	}
	return (elem);
}
bool Console::handle_generate_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		jgl::Vector2 start = jgl::compose_smaller(_interacter->pink_flag(), _interacter->blue_flag());
		jgl::Vector2 end = jgl::compose_biggest(_interacter->pink_flag(), _interacter->blue_flag());
		
		jgl::Array<size_t> elem = parse_regex_percent(tab[1]);
		if (elem.size() == 0)
			return (true);

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
		jgl::String result_string = "Area generated with parameter = " + tab[1];
		_old_entry.push_back(result_string);
	}
	else
	{
		_old_entry.push_back("Error : Bad command token number");
		_old_entry.push_back("Usage : generate [bloc regex]");
		_old_entry.push_back("Regex exemple : 1%50,2%12,3%13,4%25");
	}
	return (true);
}
bool Console::handle_replace_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 3)
	{
		jgl::Vector2 start = jgl::compose_smaller(_interacter->pink_flag(), _interacter->blue_flag());
		jgl::Vector2 end = jgl::compose_biggest(_interacter->pink_flag(), _interacter->blue_flag());

		jgl::Array<size_t> elem_base = parse_regex_simple(tab[1]);
		jgl::Array<size_t> elem_next = parse_regex_percent(tab[2]);

		if (elem_base.size() == 0 || elem_next.size() == 0)
			return (true);

		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				Node* node = _board->node(jgl::Vector2(i, j));
				if (node != nullptr)
				{
					if (elem_base.find(node->index) != elem_base.end())
					{
						size_t tmp = elem_next[jgl::generate_nbr(0, elem_next.size())];
						_board->place(jgl::Vector2(i, j), tmp, false);
					}
				}
				
			}
		start = (start / chunk_size).floor();
		end = (end / chunk_size).floor();
		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				_board->bake_chunk(jgl::Vector2(i, j));
			}
		jgl::String result_string = "Area change from " + tab[1] + " to " + tab[2];
		_old_entry.push_back(result_string);

		return (true);
	}
	else
		return (false);
}
bool Console::handle_prefab_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2 && tab[1].size() <= 4)
	{
		Prefab* new_prefab = new Prefab();
		new_prefab->save(tab[1], _board, _interacter->pink_flag(), _interacter->blue_flag());
		new_prefab->save_to_file("ressources/prefab/" + tab[1] + ".prefab");
		prefab_array.push_back(new_prefab);
		Prefab_item* new_item = new Prefab_item(new_prefab, _tileset);
		prefab_item_list.push_back(new_item);
		_inventory->tab(8)->add_item_slot(new_item);
		_inventory->tab(8)->set_geometry(_inventory->tab(7)->viewport()->anchor(), _inventory->tab(7)->area());
		_old_entry.push_back("Prefab [" + tab[1] + "] succesfully created");
		return (true);
	}
	else
	{
		_old_entry.push_back("Usage : prefab [prefab name - 0-4 letters]");
	}
	return (false);
}
bool Console::handle_ghost_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		bool state;
		if (tab[1] == "on")
			state = true;
		else if (tab[1] == "off")
			state = false;
		else
		{
			_old_entry.push_back("Usage : ghost [on / off]");
			return (true);
		}
		_player->set_ghost(state);
		if (state == true)
			_old_entry.push_back("Ghost set to on");
		else
			_old_entry.push_back("Ghost set to off");

		return (true);
	}
	else
	{
		_old_entry.push_back("Usage : ghost [on / off]");
	}
	return (false);
}
bool Console::handle_coord_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 1)
	{
		_old_entry.push_back("Player coord : " + _player->pos().str());
		return (true);
	}
	else
	{
		_old_entry.push_back("Usage : coord");
	}
	return (false);
}

bool Console::handle_console_entry(jgl::String cmd)
{
	jgl::Array<jgl::String> tab = cmd.split(" ");

	if (tab[0] == "save")
	{
		return (handle_save_command(tab));
	}
	else if (tab[0] == "load")
	{
		return (handle_load_command(tab));
	}
	else if (tab[0] == "tp")
	{
		return (handle_tp_command(tab));
	}
	else if (tab[0] == "speed")
	{
		return (handle_speed_command(tab));
	}
	else if (tab[0] == "generate")
	{
		return (handle_generate_command(tab));
	}
	else if (tab[0] == "replace")
	{
		return (handle_replace_command(tab));
	}
	else if (tab[0] == "prefab")
	{
		return (handle_prefab_command(tab));
	}
	else if (tab[0] == "coord")
	{
		return (handle_coord_command(tab));
		_old_entry.push_back(_player->pos().str());
		return (true);
	}
	else if (tab[0] == "ghost")
	{
		return (handle_ghost_command(tab));
	}
	return (false);
}

bool Console::handle_keyboard()
{
	if (jgl::get_key(jgl::key::return_key) == jgl::key_state::release || jgl::get_key(jgl::key::key_pad_return_key) == jgl::key_state::release)
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
		jgl::draw_text(_old_entry[_old_entry.size() - i - 1], pos, 16, 1, jgl::text_color::white);
	}
}