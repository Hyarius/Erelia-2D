#include "erelia.h"

Battle_mode::Battle_mode(jgl::Widget* parent)
{
	_contener = new jgl::Contener(this);
	_contener->activate();

	_pointer = new Entity(Entity_type::Player, "", 0, -1);
	_pointer->set_sprite(jgl::Vector2(static_cast<int>(Battle_node_type::cursor), 0));
	_pointer->set_tileset(engine->battle_tileset());
	_arena = nullptr;

	_active_ally = nullptr;
	_active_enemy = nullptr;

	_allies.clear();
	_enemies.clear();
	_neutrals.clear();

	_calculated = false;

	_arena_renderer = new Battle_arena_renderer(_contener);
	_arena_renderer->activate();

	_controller = new Battle_controller(_contener);
	_controller->activate();

	_face_renderer = new Battle_face_renderer(&_active_ally, &_active_enemy, _contener);
	_face_renderer->activate();
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
	engine->board()->rebake(_arena_renderer->viewport());
	_phase = Battle_phase::placement;
	_arena = p_arena;

	_arena_renderer->set_arena(p_arena);
	_arena_renderer->set_pointer(_pointer);
	_arena_renderer->set_turn_order(&_turn_order);
	_arena_renderer->set_turn_index(&_turn_index);
	_pointer->place( engine->player()->pos());
	
	_turn_order.clear();
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

	_turn_index = -1;
	_turn_order.push_back(_allies[0]);
	_turn_order.push_back(_enemies[0]);

	_arena->generate_random_start();

	size_t index = jgl::generate_nbr(0, _arena->enemy_start_pos().size());
	place(_enemies[0], _arena->enemy_start_pos()[index] + _arena->pos());
	//place(_allies[0], _arena->ally_start_pos()[0] + _arena->pos());

	_arena->rebake(_arena_renderer->viewport());
}

void Battle_mode::exit()
{
	//engine->player()->place(_pointer->pos());
	engine->change_mode(game_mode::adventure);
	engine->board()->rebake(g_application->viewport());
	delete _arena;
	_arena = nullptr;
}

void Battle_mode::place(Creature_entity* entity, jgl::Vector2 pos)
{
	Battle_node* node = _arena->absolute_battle_node(pos);
	if (node != nullptr)
	{
		if (entity->team() == Team::ally)
			_active_ally = entity;
		else
			_active_enemy = entity;
		entity->place(pos);
		node->set_occupant(entity);
	}
}

void Battle_mode::update()
{
	_pointer->update_pos(false);
	for (size_t i = 0; i < _turn_order.size(); i++)
	{
		_turn_order[i]->update();
	}
	if (_phase == Battle_phase::fight && _turn_order[_turn_index]->is_moving() == false && _calculated == false)
		check_movement();
}

void Battle_mode::end_turn()
{
	if (_turn_index != -1)
		_turn_order[_turn_index]->reset_stat();
	_turn_index += 1;
	_turn_index = _turn_index % _turn_order.size();

	if (_turn_order[_turn_index]->team() == Team::enemy)
	{
		_controller->change_phase(Battle_action_phase::enemy_turn);
	}
	else
	{
		_controller->change_phase(Battle_action_phase::base);
		check_movement();
	}
}

void Battle_mode::check_movement()
{
	static jgl::Vector2 neightbour[4] = {
		   jgl::Vector2(0, 1),
		   jgl::Vector2(1, 0),
		   jgl::Vector2(0, -1),
		   jgl::Vector2(-1, 0),
	};

	_arena->reset();

	jgl::Array<jgl::Vector2> to_calc;

	Creature_entity* entity = _turn_order[_turn_index];
	to_calc.push_back(entity->pos() - _arena->pos());
	Battle_node* tmp_node = _arena->battle_node(entity->pos() - _arena->pos());
	if (tmp_node == nullptr)
		return ;
	tmp_node->set_distance(0);
	tmp_node->set_calculated(true);
	for (size_t i = 0; i < to_calc.size(); i++)
	{
		Battle_node* actual = _arena->battle_node(to_calc[i]);
		if (actual->distance() < entity->PM().actual)
		{
			for (size_t j = 0; j < 4; j++)
			{
				jgl::Vector2 tmp = to_calc[i] + neightbour[j];
				Battle_node* node = _arena->battle_node(tmp);
				if (node != nullptr && node->occupant() == nullptr)
				{
					if (node->type() == Battle_node_type::clear && node->calculated() == false)
					{
						_arena->define_node_type(tmp, Battle_node_type::mouvement, true);
						node->set_distance(actual->distance() + 1);
						to_calc.push_back(tmp);
					}
				}
			}
		}
	}
	_arena->bake(_arena_renderer->viewport());
	_calculated = true;
}

void Battle_mode::change_phase()
{
	if (_phase == Battle_phase::placement)
	{	
		_phase = Battle_phase::fight;
		_arena->reset();
		end_turn();
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
				_arena->content()[actual_pos]->type() != Battle_node_type::border)
			{
				_pointer->move(move_delta[i], false);
				return (true);
			}
		}
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::release)
	{
		if (_active_ally != nullptr)
		{
			change_phase();
		}
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
				_arena->content()[actual_pos]->type() != Battle_node_type::border)
			{
				_pointer->move(move_delta[i], false);
				return (true);
			}
		}
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::release)
	{
		end_turn();
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
		jgl::Vector2 pos = engine->board()->screen_to_tile(g_mouse->pos, _pointer->pos());
		Battle_node* node = _arena->absolute_battle_node(pos);
		if (node != nullptr && node->type() == Battle_node_type::ally_pos)
		{
			place(_allies[0], pos);
		}
	}
	return (false);
}


void Battle_mode::cast_vision(int range)
{

}

void Battle_mode::launch_movement(jgl::Vector2 pos)
{
	Battle_node* node = _arena->absolute_battle_node(pos);
	if (node != nullptr && node->type() == Battle_node_type::mouvement)
	{
		int distance = node->distance();
		_arena->reset();
		Creature_entity* entity = _turn_order[_turn_index];
		jgl::Array<jgl::Vector2> path = _arena->pathfinding(entity->pos(), pos);

		if (path.size() != 0)
		{
			entity->set_road(path);
			entity->PM().actual -= distance;
			_calculated = false;
		}
	}
}

bool Battle_mode::handle_mouse_fight()
{
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
	{
		jgl::Vector2 pos = engine->board()->screen_to_tile(g_mouse->pos, _pointer->pos());
		launch_movement(pos);
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
	float delta = g_application->size().x / 4;
	float delta2 = g_application->size().y / 4;
	_controller->set_geometry(jgl::Vector2(0.0f, g_application->size().y - delta2), jgl::Vector2(delta, delta2));
	_arena_renderer->set_geometry(jgl::Vector2(delta, 0.0f), jgl::Vector2(g_application->size().x - delta, g_application->size().y));
	_face_renderer->set_geometry(0, jgl::Vector2(delta, g_application->size().y - delta2));
}
void Battle_mode::render()
{
	jgl::draw_text("Gamemode : Battle", 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), jgl::Vector2(0, 20) + 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}