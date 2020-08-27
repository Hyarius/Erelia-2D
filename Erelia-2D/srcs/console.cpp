#include "erelia.h"

Console::Console(jgl::Widget* p_engine) : jgl::Widget(p_engine)
{
	_complete = false;
	_cmd_index = 0;
	_entry = new jgl::Text_entry("", this);
	_entry->activate();
}

bool Console::return_funct(jgl::String text)
{
	_old_entry.push_front(text);
	return (true);
}

bool Console::handle_clear_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 1)
	{
		engine->board()->clear();
		return (return_funct("Board cleared"));
	}
	else
		return (return_funct("Usage : clear"));
}

bool Console::handle_warp_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 3)
	{
		std::string tmp = tab[2].std();
		if (tab[1] == "create")
		{
			if (engine->board()->warps().contains(tmp) != 0)
				return (return_funct("Warp [" + tmp + "] already exist"));
			else
			{
				engine->board()->add_warp(tmp, engine->player()->pos());
				return (return_funct("Creating warp [" + jgl::String(tmp) + "] at coord " + engine->player()->pos().str()));
			}
		}
		else if (tab[1] == "delete")
		{
			if (engine->board()->warps().contains(tmp) == 0)
				return (return_funct("Warp [" + tmp + "] don't exist"));
			else
			{
				engine->board()->remove_warp(tmp);
				return (return_funct("Deleting warp [" + tmp + "]"));
			}
		}
	}
	else if (tab.size() == 2)
	{
		std::string tmp = tab[1].std();
		if (engine->board()->warps().contains(tmp) != 0)
		{
			engine->player()->place(engine->board()->warp(tmp));
			return (return_funct("Player teleported to " + engine->player()->pos().str()));
		}
		else
			return (return_funct("Warp [" + tmp + "] don't exist"));
	}
	else
		return (return_funct("Usage : warp [optional : create / delete / empty to teleportation] [warp name]"));
	return (true);
}
bool Console::handle_save_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		engine->board()->save("ressources/maps/" + tab[1] + ".map");
		engine->save("ressources/save/" + engine->player()->name() + ".sav");
		return (return_funct("Saving map into file " + tab[1] + ".map"));
	}
	else
		return (return_funct("Usage : save [map path]"));
	return (true);
}
bool Console::handle_load_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		engine->board()->load("ressources/maps/" + tab[1] + ".map");
		if (jgl::check_file_exist("ressources/save/Player.sav") == true)
			engine->load("ressources/save/" + engine->player()->name() + ".sav");
		return (return_funct("Loading map from file " + tab[1] + ".map"));
	}
	else
		return (return_funct("Usage : load [map path]"));
}
bool Console::handle_tp_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 3)
	{
		jgl::Vector2 dest = jgl::Vector2(jgl::stof(tab[1]), jgl::stof(tab[2]));
		engine->player()->place(dest);
		return (return_funct("Teleport to " + engine->player()->pos().str()));
	}
	else
		return (return_funct("Usage : tp [coord X][coord Y]"));
}
bool Console::handle_speed_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2)
	{
		float speed = jgl::stof(tab[1]);
		engine->player()->set_move_speed(speed);
		return (return_funct("Setting speed to " + jgl::ftoa(speed)));
	}
	else
		return (return_funct("Usage : speed [speed value]"));
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
		jgl::Vector2 start = jgl::compose_smaller(engine->editor_mode()->interacter()->pink_flag(), engine->editor_mode()->interacter()->blue_flag());
		jgl::Vector2 end = jgl::compose_biggest(engine->editor_mode()->interacter()->pink_flag(), engine->editor_mode()->interacter()->blue_flag());
		
		jgl::Array<size_t> elem = parse_regex_percent(tab[1]);
		if (elem.size() == 0)
			return (true);

		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				size_t tmp = elem[jgl::generate_nbr(0, elem.size())];
				engine->board()->place(jgl::Vector2(i, j), tmp, false);
			}
		start = (start / CHUNK_SIZE).floor();
		end = (end / CHUNK_SIZE).floor();
		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				engine->board()->bake_chunk(jgl::Vector2(i, j));
			}
		jgl::String result_string = "Area generated with parameter = " + tab[1];
		return (return_funct(result_string));
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
		jgl::Vector2 start = jgl::compose_smaller(engine->editor_mode()->interacter()->pink_flag(), engine->editor_mode()->interacter()->blue_flag());
		jgl::Vector2 end = jgl::compose_biggest(engine->editor_mode()->interacter()->pink_flag(), engine->editor_mode()->interacter()->blue_flag());

		jgl::Array<size_t> elem_base = parse_regex_simple(tab[1]);
		jgl::Array<size_t> elem_next = parse_regex_percent(tab[2]);

		if (elem_base.size() == 0 || elem_next.size() == 0)
			return (true);

		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				Tile* tile = engine->board()->tile(jgl::Vector2(i, j));
				if (tile != nullptr)
				{
					if (elem_base.find(tile->index) != elem_base.end())
					{
						size_t tmp = elem_next[jgl::generate_nbr(0, elem_next.size())];
						engine->board()->place(jgl::Vector2(i, j), tmp, false);
					}
				}
				
			}
		start = (start / CHUNK_SIZE).floor();
		end = (end / CHUNK_SIZE).floor();
		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				engine->board()->bake_chunk(jgl::Vector2(i, j));
			}
		jgl::String result_string = "Area change from " + tab[1] + " to " + tab[2];
		return (return_funct(result_string));
	}
	else
		return (false);
}
bool Console::handle_prefab_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 2 && tab[1].size() <= 4)
	{
		Prefab* new_prefab = new Prefab();
		new_prefab->save(tab[1], engine->board(), engine->editor_mode()->interacter()->pink_flag(), engine->editor_mode()->interacter()->blue_flag());
		new_prefab->save_to_file("ressources/prefab/" + tab[1] + ".prefab");
		prefab_array.push_back(new_prefab);
		Prefab_item* new_item = new Prefab_item(new_prefab);
		prefab_item_list.push_back(new_item);
		engine->editor_mode()->inventory()->tab(9)->add_item_slot(new_item);
		engine->editor_mode()->inventory()->tab(9)->set_geometry(engine->editor_mode()->inventory()->tab(7)->viewport()->anchor(), engine->editor_mode()->inventory()->tab(7)->area());
		return (return_funct("Prefab [" + tab[1] + "] succesfully created"));
	}
	else
		return (return_funct("Usage : prefab [prefab name - 0-4 letters]"));
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
			return (return_funct("Usage : ghost [on / off]"));

		engine->player()->set_ghost(state);
		if (state == true)
		{
			if (engine->board()->node(engine->player()->pos().round()) != nullptr)
				engine->board()->node(engine->player()->pos())->set_occupant(engine->player());
			return (return_funct("Ghost set to on"));
		}
		else
		{
			if (engine->board()->node(engine->player()->pos().round()) != nullptr)
				engine->board()->node(engine->player()->pos())->set_occupant(nullptr);
			return (return_funct("Ghost set to off"));
		}
	}
	else
		return (return_funct("Usage : ghost [on / off]"));
}
bool Console::handle_coord_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() == 1)
		return (return_funct("Player coord : " + jgl::itoa(static_cast<int>(engine->player()->pos().x)) + " / " + jgl::itoa(static_cast<int>(engine->player()->pos().y))));
	else
		return (return_funct("Usage : coord"));
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
				return (return_funct("Usage : link [create / delete] - if creating a link, add \"dual\" after the create to create a double way link"));
			engine->board()->add_link(engine->editor_mode()->interacter()->pink_flag(), engine->editor_mode()->interacter()->blue_flag(), tmp);
			return (return_funct("Creating link from " + engine->editor_mode()->interacter()->pink_flag().str() + " to " + engine->editor_mode()->interacter()->blue_flag().str()));
		}
		else if (tab[1] == "delete")
		{
			Node* a = engine->board()->node(engine->editor_mode()->interacter()->pink_flag());
			if (a != nullptr && a->link() != nullptr)
			{
				Link* link = a->link();
				engine->board()->remove_link(engine->player()->pos());
				return (return_funct("Deleting link from " + link->a().str() + " to " + link->b().str()));
			}
			else
				return (return_funct("No link to delete"));
		}
		else
			return (return_funct("Usage : link [create / delete] - if creating a link, add \"dual\" after the create to create a double way link"));
	}
	else
		return (return_funct("Usage : link [create / delete] - if creating a link, add \"dual\" after the create to create a double way link"));
}

bool Console::handle_npc_command(jgl::Array<jgl::String>& tab)
{
	if (tab.size() >= 2)
	{
		if (engine->editor_mode()->interacter()->selected_entity() == nullptr)
			return (return_funct("No entity selected"));
		if (tab[1] == "movement")
		{
			if (tab.size() == 3)
			{
				int tmp = jgl::stoi(tab[2]);
				if (tmp == -1 || tmp == 0 || tmp == 1)
				{
					Entity_movement type = static_cast<Entity_movement>(tmp);
					if (engine->editor_mode()->interacter()->selected_entity() != nullptr && engine->editor_mode()->interacter()->selected_entity()->movement_type() != type)
					{
						engine->editor_mode()->interacter()->selected_entity()->set_movement_type(type);
						if (type == Entity_movement::errant)
							engine->editor_mode()->interacter()->selected_entity()->set_movement_range(5);
						else if (type == Entity_movement::controled)
						{
							engine->editor_mode()->interacter()->selected_entity()->check_point().clear();
							engine->editor_mode()->interacter()->selected_entity()->add_check_point(engine->editor_mode()->interacter()->selected_entity()->starting_pos());
						}
						return (return_funct(engine->editor_mode()->interacter()->selected_entity()->name() + " movement type set to " + (tmp == -1 ? "Errant" : (tmp == 0 ? "Immobile" : "Defined path"))));
					}
					else if (engine->editor_mode()->interacter()->selected_entity()->movement_type() == type)
						return (return_funct(jgl::String("Selected entity movement type already defined to ") + (tmp == -1 ? "Errant" : (tmp == 0 ? "Immobile" : "Defined path"))));
					else
						return (return_funct("No selected entity to edit : command ignored"));
				}
				else
					return (return_funct("Usage : npc movement [-1 : Errant / 0 : Fix / 1 : Defined path]"));
			}
			else
				return (return_funct("Usage : npc movement [-1 : Errant / 0 : Fix / 1 : Defined path]"));
		}
		if (tab[1] == "range")
		{
			if (engine->editor_mode()->interacter()->selected_entity()->movement_type() != Entity_movement::errant)
				return (return_funct("Entity movement type isn't errant. Use [npc movement 0] to set it"));
			int tmp = jgl::stoi(tab[2]);
			if (tmp <= 0)
				return (return_funct("Entity movement range can't be negative or null"));
			engine->editor_mode()->interacter()->selected_entity()->set_movement_range(tmp);
			return (return_funct("Entity movement range set to " + jgl::itoa(tmp)));
		}
		if (tab[1] == "reset")
		{
			if (engine->editor_mode()->interacter()->selected_entity()->movement_type() == Entity_movement::errant)
				engine->editor_mode()->interacter()->selected_entity()->set_movement_range(5);
			else if (engine->editor_mode()->interacter()->selected_entity()->movement_type() == Entity_movement::controled)
			{
				engine->editor_mode()->interacter()->selected_entity()->check_point().clear();
				engine->editor_mode()->interacter()->selected_entity()->add_check_point(engine->editor_mode()->interacter()->selected_entity()->starting_pos());
			}
			engine->editor_mode()->interacter()->selected_entity()->calc_road_to(engine->editor_mode()->interacter()->selected_entity()->starting_pos());
			return (return_funct("Entity reseted"));
		}
		else
			return (return_funct("Usage : npc [movement / range / reset]"));
	}
	else
		return (return_funct("Usage : npc [movement / range / reset]"));
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


void Console::start()
{
	activate();
	entry()->select();
	_complete = false;
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
		_complete = true;
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
	_entry->set_geometry(jgl::Vector2(5.0f, p_area.y - 50), jgl::Vector2(p_area.x * 0.75f, 45.0f));
}

void Console::render()
{
	if (_old_entry.size() != 0)
	{
		for (int i = _old_entry.size() - 1; i >= 0; i--)
		{
			int text_size = 20;
			jgl::Vector2 tmp = _entry->anchor() - jgl::Vector2(0.0f, text_size * 1.4f);
			jgl::Vector2 pos = tmp + jgl::Vector2(20.0f, i * -(text_size * 1.4f));
			jgl::draw_text(_old_entry[i], pos, text_size, 2, jgl::text_color::white);
		}
	}
}