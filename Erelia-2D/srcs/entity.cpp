#include "erelia.h"

Entity::Entity() : Entity(Entity_type::entity, "Unnamed", 0, 0)
{
	
}

Entity::Entity(Entity_type p_type, jgl::String p_name, jgl::Vector2 p_pos, jgl::Vector2 p_sprite)
{
	_tileset = nullptr;
	_type = p_type;
	_name = p_name;
	_look_dir = Entity_direction::south;
	_wait_time = 50;
	_sprite = p_sprite;
	_pos = p_pos;
	_old_pos = _pos;
	_destination = _pos;
	_direction = 0;
	_did_tp = false;
	_in_motion = false;
	_move_tick = 150;
	_move_speed = 1.0f;
	_total_tick = _move_tick / _move_speed;
	_movement_range = 5;
	_starting_pos = p_pos;
	_road_index = 0;
	_check_point_index = 1;
	_movement_type = Entity_movement::fix;
}

void Entity::update()
{
	if (is_moving() == true)
	{
		update_pos();
	}
	if (is_interacting() == true)
		return;
	if (is_moving() == false)
	{
		if (_road.size() != 0 && _road_index != _road.size())
		{
			jgl::Vector2 tmp = (_road[_road_index] - _pos).round();
			if (can_move(tmp) == true)
			{
				move(tmp);
				_road_index++;
			}
		}
		else
		{
			if (_movement_type == Entity_movement::errant)
			{
				size_t x = static_cast<size_t>(_starting_pos.x);
				size_t y = static_cast<size_t>(_starting_pos.y);
				jgl::Vector2 dest = jgl::Vector2(
					jgl::generate_nbr(x - _movement_range, x + _movement_range),
					jgl::generate_nbr(y - _movement_range, y + _movement_range)
				);
				size_t i = 0;
				while (engine->board()->node(dest)->tile()->type == OBSTACLE || engine->board()->node(dest)->tile()->type == JUMPING && i < 20)
				{
					dest = jgl::Vector2(
						jgl::generate_nbr(x - _movement_range, x + _movement_range),
						jgl::generate_nbr(y - _movement_range, y + _movement_range)
					);
					i++;
				}

				calc_road_to(dest);
			}
			else if (_movement_type == Entity_movement::controled)
			{
				_check_point_index = (_check_point_index + 1) % _check_point.size();
				calc_next_road();
			}
		}
	}
}

void Entity::place(jgl::Vector2 p_pos)
{
	_pos = p_pos.round();
	_did_tp = true;
	_in_motion = true;
	_total_tick = 0;
	_last_tick = static_cast<float>(g_time);
	_actual_tick = _last_tick;
}

void Entity::move(jgl::Vector2 delta, bool edit)
{
	if (is_active() == true)
		return;

	if (delta.x > 0)
		_look_dir = Entity_direction::east;
	else if (delta.x < 0)
		_look_dir = Entity_direction::west;
	else if (delta.y > 0)
		_look_dir = Entity_direction::south;
	else if (delta.y < 0)
		_look_dir = Entity_direction::north;
	else
	{
		_look_dir = Entity_direction::south;
		_in_motion = false;
		_direction = 0;
		_did_tp = false;
		return;
	}
	_destination = _pos + delta;
	_old_pos = _pos;
	_in_motion = true;
	_did_tp = false;
	_total_tick = 0;
	_last_tick = static_cast<float>(g_time);
	_actual_tick = _last_tick;
	if (edit == true)
	{
		if (engine->index_mode() == game_mode::battle)
		{
			Battle_node *tmp1 = engine->battle_mode()->arena()->absolute_battle_node(_pos + delta);
			if (tmp1 != nullptr)
				tmp1->set_occupant(this);
		}
		else if (engine->index_mode() == game_mode::editor || engine->index_mode() == game_mode::adventure)
		{
			Node* tmp1 = engine->board()->node(_pos + delta);
			if (tmp1 != nullptr)
				tmp1->set_occupant(this);
		}
		
	}
	_direction = delta / (_move_tick / _move_speed);
}
void Entity::update_pos(bool edit)
{
	jgl::Vector2 rpos = _pos.round();

	if (_in_motion == true)
	{
		if (_last_tick + _wait_time > g_time)
			_in_motion = true;
		else
			_in_motion = false;
	}

	if (_direction == 0 && is_static() == true)
		return;

	_actual_tick = static_cast<float>(g_time);
	float delta = _actual_tick - _last_tick;

	if (_total_tick + delta > (_move_tick / _move_speed))
	{
		delta = (_move_tick / _move_speed) - _total_tick;
	}
	_total_tick += delta;
	_last_tick = _actual_tick;
	jgl::Vector2 delta_pos = _direction * delta;
	_pos += delta_pos;
	if (_total_tick >= (_move_tick / _move_speed))
	{
		if (_did_tp == false)
			_pos = _destination;
		_direction = 0;
	}

	if (rpos != _pos.round() && edit == true)
	{
		Base_node* tmp1 = nullptr;
		Base_node* tmp2 = nullptr;
		if (engine->index_mode() == game_mode::battle)
		{
			tmp1 = engine->battle_mode()->arena()->absolute_battle_node(rpos);
			tmp2 = engine->battle_mode()->arena()->absolute_battle_node(_pos.round());
		}
		else if (engine->index_mode() == game_mode::editor || engine->index_mode() == game_mode::adventure)
		{
			tmp1 = engine->board()->node(rpos);
			tmp2 = engine->board()->node(_pos.round());
		}
		if (tmp1 != nullptr)
			tmp1->set_occupant(nullptr);
		if (tmp2 != nullptr)
			tmp2->set_occupant(this);
	}
}

jgl::Array<size_t> tmp_delta[9]
{
	{1,0,1,2},
	{},
	{},
	{1,0,1,2},
	{},
	{},
	{0,1,0,1},
	{},
	{0,1,0,1},
};

bool Entity::is_interacting()
{
	if (engine->active_mode() == nullptr)
		return (false);
	if (engine->index_mode() == game_mode::editor)
		return (engine->editor_mode()->interacter()->source() == this || engine->editor_mode()->interacter()->target() == this);
	else if (engine->index_mode() == game_mode::adventure)
		return (engine->adventure_mode()->interacter()->source() == this || engine->adventure_mode()->interacter()->target() == this);
	else
		return (false);
}

bool Entity::is_pointed(jgl::Vector2 base_pos)
{
	jgl::Vector2 pos = engine->board()->tile_to_screen(_pos, base_pos);

	if (jgl::is_middle(pos, g_mouse->pos - engine->board()->active_viewport()->anchor(), pos + node_size) == true)
		return (true);
	return (false);
}

void Entity::render_grass(jgl::Viewport* p_viewport, jgl::Vector2 base_pos)
{
	if (_direction.x != 0)
	{
		if (engine->board()->node(_destination) != nullptr && engine->board()->node(_destination)->tile() != nullptr && (engine->board()->node(_destination)->tile()->type & GRASS_TILE) == GRASS_TILE)
			engine->charset()->draw(jgl::Vector2(1, 41), engine->board()->tile_to_screen(_destination.round(), base_pos), node_size, 1.0f, p_viewport);
		if (engine->board()->node(_old_pos) != nullptr && engine->board()->node(_old_pos)->tile() != nullptr && (engine->board()->node(_old_pos)->tile()->type & GRASS_TILE) == GRASS_TILE)
			engine->charset()->draw(jgl::Vector2(1, 41), engine->board()->tile_to_screen(_old_pos.round(), base_pos), node_size, 1.0f, p_viewport);
	}
	else if (_direction.y != 0)
	{
		jgl::Vector2 start = (_direction.y > 0 ? _destination : _old_pos);
		while (start.y > _pos.y)
		{
			if (engine->board()->node(start + jgl::Vector2(0.0f, 0.1f)) != nullptr && engine->board()->node(start + jgl::Vector2(0.0f, 0.1f))->tile() != nullptr && (engine->board()->node(start + jgl::Vector2(0.0f, 0.1f))->tile()->type & GRASS_TILE) == GRASS_TILE)
				engine->charset()->draw(jgl::Vector2(1, 41), engine->board()->tile_to_screen(start, base_pos), node_size, 1.0f, p_viewport);
			start.y -= 0.5f;
		}
	}
	else
	{
		if (engine->board()->node(_pos) != nullptr && engine->board()->node(_pos)->tile() != nullptr && (engine->board()->node(_pos)->tile()->type & GRASS_TILE) == GRASS_TILE)
			engine->charset()->draw(jgl::Vector2(1, 41), engine->board()->tile_to_screen(_pos.round(), base_pos), node_size, 1.0f, p_viewport);
	}
}

void Entity::render(jgl::Viewport* p_viewport, jgl::Vector2 base_pos)
{
	jgl::Vector2 pos = engine->board()->tile_to_screen(_pos, base_pos);
	jgl::Vector2 dir_delta = jgl::Vector2(static_cast<int>(_look_dir), 0);
	jgl::Vector2 delta = 0;
	size_t value = 0;
	
	if (_tileset != nullptr)
	{
		if (_sprite != -1)
			_tileset->draw(_sprite, pos, node_size, 1.0f, p_viewport);
	}
	else
	{
		if (is_moving() == true)
			value += tmp_delta[static_cast<size_t>(_look_dir)][jgl::get_frame_state(tmp_delta[static_cast<size_t>(_look_dir)].size())];
		else if (_look_dir == Entity_direction::north || _look_dir == Entity_direction::south)
			value += 1;
		delta.x += value;
		if (_sprite.y >= 0)
		{
			engine->charset()->draw(_sprite + delta + dir_delta, pos, node_size, 1.0f, p_viewport);
			render_grass(p_viewport, base_pos);
		}

		if (_type != Entity_type::Player && is_pointed(base_pos) == true)
			jgl::draw_centred_text(_name, pos + node_size / 2 - jgl::Vector2(0, node_size), 16, 1, 1.0f, jgl::text_color::white);
	}
	
}

bool Entity::can_move(jgl::Vector2 delta)
{
	Node* tmp = engine->board()->node((this->pos() + delta).round());

	if (tmp != nullptr && tmp->occuped() == false &&
		engine->board()->can_acces(this->pos(), delta) == true)
		return (true);
	return (false);
}

void Entity::remove_check_point(jgl::Vector2 pos)
{
	if (pos == _starting_pos)
		return;
	auto tmp = _check_point.find(pos);
	if (tmp != _check_point.end())
	{
		_check_point.erase(tmp);
	}
}

void Entity::calc_next_road()
{
	if (_check_point.size() <= 1)
		_road = jgl::Array<jgl::Vector2>();
	else
		calc_road_to(_check_point[_check_point_index].round());
}

void Entity::calc_road_to(jgl::Vector2 destination)
{
	_road = engine->board()->pathfinding(_pos.round(), destination);
	_road_index = 1;
}

void Entity::return_starting_position()
{
	_road = engine->board()->pathfinding(_pos.round(), _starting_pos.round());
	_road_index = 1;
}