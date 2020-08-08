#include "erelia.h"

Console::Console(Editor_inventory* p_inventory, jgl::Sprite_sheet* p_tileset, Editor_interact* p_interacter, Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_tileset = p_tileset;
	_board = p_board;
	_player = p_player;
	_inventory = p_inventory;
	_interacter = p_interacter;
	_cmd_index = 0;
	_entry = new jgl::Text_entry("", this);
	_entry->activate();
}

bool Console::handle_clear_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 1)
	{
		_board->clear();
	}
	else
		_old_entry.push_front("Usage : clear");
	return (true);
}

bool Console::handle_warp_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 3)
	{
		std::string tmp = tab[2].std();
		if (tab[1] == "create")
		{
			if (_board->warps().contains(tmp) != 0)
				_old_entry.push_front("Warp [" + tmp + "] already exist");
			else
			{
				_board->add_warp(tmp, _player->pos());
				jgl::String text = "Creating warp [" + jgl::String(tmp) + "] at coord " + _player->pos().str();
				_old_entry.push_front(text);
			}
		}
		else if (tab[1] == "delete")
		{
			if (_board->warps().contains(tmp) == 0)
				_old_entry.push_front("Warp [" + tmp + "] don't exist");
			else
			{
				_board->remove_warp(tmp);
				_old_entry.push_front("Deleting warp [" + tmp + "]");
			}
		}
	}
	else if (tab.size() == 2)
	{
		std::string tmp = tab[1].std();
		if (_board->warps().contains(tmp) != 0)
		{
			_player->place(_board->warp(tmp));
			_old_entry.push_front("Player teleported to " + _player->pos().str());
		}
		else
			_old_entry.push_front("Warp [" + tmp + "] don't exist");
	}
	else
		_old_entry.push_front("Usage : warp [optional : create / delete / empty to teleportation] [warp name]");
	return (true);
}
bool Console::handle_save_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		_board->save("ressources/maps/" + tab[1] + ".map");
		_old_entry.push_front("Saving map into file " + tab[1] + ".map");
	}
	else
		_old_entry.push_front("Usage : save [map path]");
	return (true);
}
bool Console::handle_load_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		_board->load("ressources/maps/" + tab[1] + ".map");
		_old_entry.push_front("Loading map from file " + tab[1] + ".map");
	}
	else
		_old_entry.push_front("Usage : load [map path]");
	return (true);
}
bool Console::handle_tp_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 3)
	{
		jgl::Vector2 dest = jgl::Vector2(jgl::stof(tab[1]), jgl::stof(tab[2]));
		_player->place(dest);
		_old_entry.push_front("Teleport to " + _player->pos().str());
	}
	else
		_old_entry.push_front("Usage : tp [coord X][coord Y]");
	return (true);
}
bool Console::handle_speed_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		float speed = jgl::stof(tab[1]);
		_old_entry.push_front("Setting speed to " + jgl::ftoa(speed));
		_player->set_move_speed(speed);
	}
	else
		_old_entry.push_front("Usage : speed [speed value]");
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
			_old_entry.push_front("Error : Token number error in regex");
			_old_entry.push_front("Usage : generate [bloc regex]");
			_old_entry.push_front("Regex exemple : 1%50,2%12,3%13,4%25");
			return (jgl::Array<size_t>());
		}
		int index = jgl::stoi(regex_part[0]);
		int nb = jgl::stoi(regex_part[1]);
		if (index < 0 || index >= static_cast<int>(tile_array.size()))
		{
			_old_entry.push_front("Error : tile index out of range");
			_old_entry.push_front("Usage : generate [bloc regex]");
			_old_entry.push_front("Error in regex : Bad index");
			return (jgl::Array<size_t>());
		}
		else if (nb <= 0)
		{
			_old_entry.push_front("Error : Percent negative");
			_old_entry.push_front("Usage : generate [bloc regex]");
			_old_entry.push_front("Error in regex : Bad probability value");
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
		if (index < 0 || index >= static_cast<int>(tile_array.size()))
		{
			_old_entry.push_front("Error : tile index out of range");
			_old_entry.push_front("Usage : generate [bloc regex]");
			_old_entry.push_front("Error in regex : Bad index");
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
		_old_entry.push_front(result_string);
	}
	else
	{
		_old_entry.push_front("Error : Bad command token number");
		_old_entry.push_front("Usage : generate [bloc regex]");
		_old_entry.push_front("Regex exemple : 1%50,2%12,3%13,4%25");
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
				Tile* tile = _board->tile(jgl::Vector2(i, j));
				if (tile != nullptr)
				{
					if (elem_base.find(tile->index) != elem_base.end())
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
		_old_entry.push_front(result_string);

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
		_inventory->tab(9)->add_item_slot(new_item);
		_inventory->tab(9)->set_geometry(_inventory->tab(7)->viewport()->anchor(), _inventory->tab(7)->area());
		_old_entry.push_front("Prefab [" + tab[1] + "] succesfully created");
		return (true);
	}
	else
	{
		_old_entry.push_front("Usage : prefab [prefab name - 0-4 letters]");
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
			_old_entry.push_front("Usage : ghost [on / off]");
			return (true);
		}
		_player->set_ghost(state);
		if (state == true)
			_old_entry.push_front("Ghost set to on");
		else
			_old_entry.push_front("Ghost set to off");

		return (true);
	}
	else
	{
		_old_entry.push_front("Usage : ghost [on / off]");
	}
	return (false);
}
bool Console::handle_coord_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 1)
	{
		_old_entry.push_front("Player coord : " + jgl::itoa(static_cast<int>(_player->pos().x)) + " / " + jgl::itoa(static_cast<int>(_player->pos().y)));
		return (true);
	}
	else
	{
		_old_entry.push_front("Usage : coord");
	}
	return (false);
}
bool Console::handle_link_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() >= 2)
	{
		if (tab[1] == "create")
		{
			bool tmp = false;
			if (tab.size() == 3 && tab[2] == "dual")
				tmp = true;
			else if (tab.size() == 3)
			{
				_old_entry.push_front("Usage : link [create / delete] - if creating a link, add \"dual\" after the create to create a double way link");
				return (true);
			}
			_board->add_link(_interacter->pink_flag(), _interacter->blue_flag(), tmp);
			_old_entry.push_front("Creating link from " + _interacter->pink_flag().str() + " to " + _interacter->blue_flag().str());
			return (true);
		}
		else if (tab[1] == "delete")
		{
			Node* a = _board->node(_interacter->pink_flag());
			if (a != nullptr && a->link() != nullptr)
			{
				Link* link = a->link();
				_old_entry.push_front("Deleting link from " + link->a().str() + " to " + link->b().str());
				_board->remove_link(_player->pos());
			}
			else
				_old_entry.push_front("No link to delete");
			return (true);
		}
		else
			_old_entry.push_front("Usage : link [create / delete] - if creating a link, add \"dual\" after the create to create a double way link");
	}
	else
	{
		_old_entry.push_front("Usage : link [create / delete] - if creating a link, add \"dual\" after the create to create a double way link");
	}
	return (true);
}
bool Console::handle_npc_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() >= 3)
	{
		if (tab[1] == "create" && tab.size() == 4)
		{
			jgl::Vector2 sprite = jgl::Vector2(0, jgl::stoi(tab[3]));
			NPC* new_npc = new NPC(tab[2], _interacter->pink_flag(), _board->charset(), sprite);
			_board->add_npc(new_npc);
		}
		else if (tab[1] == "copy")
		{

		}
		else if (tab[1] == "delete")
		{
			_board->remove_npc(tab[2]);
		}
		return (true);
	}
	else
	{
		_old_entry.push_front("Usage : npc [create / copy / delete] [NPC name]");
		return (true);
	}
}

bool Console::handle_console_entry(jgl::String cmd)
{
	jgl::Array<jgl::String> tab = cmd.split(" ");

	if (tab[0] == "save")
		return (handle_save_command(tab));
	else if (tab[0] == "load")
		return (handle_load_command(tab));
	else if (tab[0] == "tp")
		return (handle_tp_command(tab));
	else if (tab[0] == "speed")
		return (handle_speed_command(tab));
	else if (tab[0] == "generate")
		return (handle_generate_command(tab));
	else if (tab[0] == "replace")
		return (handle_replace_command(tab));
	else if (tab[0] == "prefab")
		return (handle_prefab_command(tab));
	else if (tab[0] == "coord")
		return (handle_coord_command(tab));
	else if (tab[0] == "ghost")
		return (handle_ghost_command(tab));
	else if (tab[0] == "clear")
		return (handle_clear_command(tab));
	else if (tab[0] == "warp")
		return (handle_warp_command(tab));
	else if (tab[0] == "link")
		return (handle_link_command(tab));
	else if (tab[0] == "npc")
		return (handle_npc_command(tab));
	return (false);
}

bool Console::handle_keyboard()
{
	if (jgl::get_key(jgl::key::return_key) == jgl::key_state::release || jgl::get_key(jgl::key::key_pad_return_key) == jgl::key_state::release)
	{
		_old_cmd.push_front(_entry->text());
		if (handle_console_entry(_entry->text()) == false)
			_old_entry.push_front(_entry->text());
		_entry->set_text("");
		_entry->select();
		_cmd_index = 0;
	}
	if (jgl::get_key(jgl::key::up_arrow) == jgl::key_state::release)
	{
		if (_cmd_index < _old_cmd.size())
		{
			_cmd_index++;
			_entry->set_text(_old_cmd[_cmd_index - 1]);
		}
	}
	if (jgl::get_key(jgl::key::down_arrow) == jgl::key_state::release)
	{
		if (_cmd_index > 0)
			_cmd_index--;

		if (_cmd_index > 0)
			_entry->set_text(_old_cmd[_cmd_index - 1]);
		else
			_entry->set_text("");
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
	if (_old_entry.size() != 0)
	{
		for (int i = _old_entry.size() - 1; i >= 0; i--)
		{
			jgl::Vector2 tmp = _entry->anchor() - jgl::Vector2(0.0f, 30.0f);
			jgl::Vector2 pos = tmp + jgl::Vector2(20.0f, i * -21.0f);
			jgl::draw_text(_old_entry[i], pos, 16, 1, jgl::text_color::white);
		}
	}
}