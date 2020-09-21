#include "erelia.h"

Battle_mode::Battle_mode(jgl::Widget* parent)
{
	_contener = new jgl::Contener(this);
	_contener->activate();

	_pointer = new Entity(Entity_type::Player, "", 0, -1);
	_pointer->set_sprite(jgl::Vector2(-1, -1));
	_pointer->set_tileset(engine->battle_tileset());
	_arena = nullptr;

	_allies.clear();
	_enemies.clear();
	_neutrals.clear();
}

void Battle_mode::add_creature(Creature_entity* creature)
{
	if (creature == nullptr)
		return;

	if (creature->team() == Team::ally)
		_allies.push_back(creature);
	else if (creature->team() == Team::enemy)
		_enemies.push_back(creature);
	else
		_neutrals.push_back(creature);
}

uint32_t tmp_time;

void Battle_mode::start(Battle_arena* p_arena, Team_comp first, Team_comp second)
{

	_phase = Battle_phase::placement;
	_arena = p_arena;

	_pointer->place(engine->player()->pos());
	
	_turn_order.clear();
	_ally_start_pos.clear();
	_enemy_start_pos.clear();
	_allies.clear();
	_enemies.clear();
	_neutrals.clear();

	for (size_t i = 0; i < first.unit.size(); i++)
		if (first.unit[i] != nullptr)
		{
			add_creature(first.unit[i]);
		}
	for (size_t i = 0; i < second.unit.size(); i++)
		if (second.unit[i] != nullptr)
		{
			add_creature(second.unit[i]);
		}

	_turn_order.push_back(_allies[0]);
	_turn_order.push_back(_enemies[0]);

	jgl::Array<jgl::Vector2> accessible_node = _arena->accessible_node();
	for (size_t i = 0; i < 10; i++)
	{
		size_t index = jgl::generate_nbr(0, accessible_node.size());
		_ally_start_pos.push_back(accessible_node[index]);
		_arena->define_node_type(accessible_node[index], Battle_node_type::ally_pos, false);
		accessible_node.erase(index);
	}
	for (size_t i = 0; i < 10; i++)
	{
		size_t index = jgl::generate_nbr(0, accessible_node.size());
		_enemy_start_pos.push_back(accessible_node[index]);
		_arena->define_node_type(accessible_node[index], Battle_node_type::enemy_pos, false);
		accessible_node.erase(index);
	}

	size_t index = jgl::generate_nbr(0, _enemy_start_pos.size());
	place(_enemies[0], _enemy_start_pos[index] + _arena->pos());

	_arena->rebake();
}

void Battle_mode::exit()
{
	engine->player()->place(_pointer->pos());
	engine->change_mode(game_mode::adventure);
	delete _arena;
	_arena = nullptr;
}

void Battle_mode::place(Creature_entity* entity, jgl::Vector2 pos)
{
	Battle_node* node = _arena->absolute_battle_node(pos);
	if (node != nullptr)
	{
		entity->place(pos);
		node->occupant = entity;
	}
}

void Battle_mode::update()
{
	_pointer->update_pos(false);
}

void Battle_mode::change_phase()
{
	if (_phase == Battle_phase::placement)
	{
		for (size_t i = 0; i < _ally_start_pos.size(); i++)
			_arena->define_node_type(_ally_start_pos[i], Battle_node_type::clear, false);
		for (size_t i = 0; i < _enemy_start_pos.size(); i++)
			_arena->define_node_type(_enemy_start_pos[i], Battle_node_type::clear, false);
		_phase = Battle_phase::fight;
		_arena->bake();
	}
	else if (_phase == Battle_phase::fight)
		_phase = Battle_phase::exit;
}

bool Battle_mode::handle_keyboard_placement()
{
	static jgl::key key_value[4] = { jgl::key::s, jgl::key::w, jgl::key::a, jgl::key::d };
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };

	for (size_t i = 0; i < 4; i++)
	{
		if (jgl::get_key(key_value[i]) == jgl::key_state::down)
		{
			jgl::Vector2 actual_pos = _pointer->pos() - _arena->pos() + move_delta[i];
			if (_pointer->is_static() == true && _arena->content().count(actual_pos) != 0 &&
				_arena->content()[actual_pos]->type != Battle_node_type::border)
			{
				_pointer->move(move_delta[i], false);
				return (true);
			}
		}
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::release)
	{
		change_phase();
		return (true);
	}
	else
		return (false);
}

bool Battle_mode::handle_keyboard_fight()
{
	static jgl::key key_value[4] = { jgl::key::s, jgl::key::w, jgl::key::a, jgl::key::d };
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };

	for (size_t i = 0; i < 4; i++)
	{
		if (jgl::get_key(key_value[i]) == jgl::key_state::down)
		{
			jgl::Vector2 actual_pos = _pointer->pos() - _arena->pos() + move_delta[i];
			if (_pointer->is_static() == true && _arena->content().count(actual_pos) != 0 &&
				_arena->content()[actual_pos]->type != Battle_node_type::border &&
				_arena->content()[actual_pos]->type != Battle_node_type::obstacle)
			{
				_pointer->move(move_delta[i], false);
				return (true);
			}
		}
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::release)
	{
		change_phase();
		return (true);
	}
	else
		return (false);
}

bool Battle_mode::handle_keyboard()
{
	if (_phase == Battle_phase::placement)
		return (handle_keyboard_placement());
	else if (_phase == Battle_phase::fight)
		return (handle_keyboard_fight());
	else
	{
		exit();
		return (true);
	}
}

bool Battle_mode::handle_mouse_placement()
{
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
	{
		jgl::Vector2 pos = screen_to_tile(g_mouse->pos, _pointer->pos());
		Battle_node* node = _arena->absolute_battle_node(pos);
		if (node != nullptr && node->type == Battle_node_type::ally_pos)
		{
			place(_allies[0], pos);
		}
	}
	return (false);
}

bool Battle_mode::handle_mouse_fight()
{
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
	{
		jgl::Vector2 pos = screen_to_tile(g_mouse->pos, _pointer->pos());
		jgl::Array<jgl::Vector2> path = _arena->pathfinding(_allies[0]->pos(), pos);
		std::cout << "Path : ";
		if (path.size() == 0)
			std::cout << "No path" << std::endl;
		else
		{
			for (size_t i = 0; i < path.size(); i++)
			{
				if (i != 0)
					std::cout << " - ";
				std::cout << path[i];
			}
			std::cout << std::endl;
		}
	}
	return (false);
}

bool Battle_mode::handle_mouse()
{
	if (_phase == Battle_phase::placement)
		return (handle_mouse_placement());
	else if (_phase == Battle_phase::fight)
		return (handle_mouse_fight());
	else
		return (false);
}

void Battle_mode::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}
void Battle_mode::render()
{
	engine->board()->render(_viewport, _pointer->pos());
	if (_arena != nullptr)
	{
		_arena->render(_viewport, _pointer->pos() - 0.5f);
		jgl::Vector2 start_pos = tile_to_screen(_arena->pos() - 1, _pointer->pos());
		jgl::Vector2 end_pos = tile_to_screen(_arena->pos() + _arena->size() + 1, _pointer->pos());
		jgl::fill_rectangle(0, jgl::Vector2(g_application->size().x, start_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(0.0f, end_pos.y) , jgl::Vector2(g_application->size().x, g_application->size().y - end_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(0.0f, start_pos.y), jgl::Vector2(start_pos.x, end_pos.y - start_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(end_pos.x, start_pos.y), jgl::Vector2(g_application->size().x - end_pos.x, end_pos.y - start_pos.y), jgl::Color(0, 0, 0, 200));
	}
	_pointer->render(_viewport, _pointer->pos());
	for (size_t i = 0; i < _turn_order.size(); i++)
		_turn_order[i]->render(_viewport, _pointer->pos());
	jgl::draw_text("Gamemode : Battle", 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), jgl::Vector2(0, 20) + 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}