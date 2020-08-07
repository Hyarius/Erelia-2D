#include "erelia.h"

Entity::Entity(jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite)
{
	_look_dir = Entity_direction::south;
	_wait_time = 50;
	_charset = p_charset;
	_sprite = p_sprite;
	_pos = p_pos;
	_direction = 0;
	_did_tp = false;
	_in_motion = false;
	_move_tick = 150;
	_move_speed = 1.0f;
	_total_tick = _move_tick / _move_speed;
}


bool Entity::can_move(Board* board, jgl::Vector2 delta)
{
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };
	static node_type rev_type[4] = { DOWN_WALKABLE, UP_WALKABLE, LEFT_WALKABLE, RIGHT_WALKABLE };
	static node_type type[4] = { UP_WALKABLE, DOWN_WALKABLE, RIGHT_WALKABLE, LEFT_WALKABLE };
	size_t i = 0;
	for (; i < 4; i++)
		if (delta == move_delta[i])
			break;
	if (is_active() == false && i != 4)
	{
		Node* tmp = board->node((this->pos() + delta).round());
		Node* actual = board->node((this->pos()).round());
		if (tmp != nullptr && actual != nullptr &&
			tmp->tile() != nullptr && actual->tile() != nullptr &&
			tmp->occuped() == false &&
			(tmp->tile()->type & rev_type[i]) == rev_type[i] &&
			(actual->tile()->type & type[i]) == type[i])
			return (true);
	}
	return (false);
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

void Entity::move(jgl::Vector2 delta)
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
	_in_motion = true;
	_did_tp = false;
	_total_tick = 0;
	_last_tick = static_cast<float>(g_time);
	_actual_tick = _last_tick;
	_direction = delta / (_move_tick / _move_speed);
}
void Entity::update_pos()
{
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
		_direction = 0;
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

void Entity::render(jgl::Vector2 player_pos, jgl::Viewport* p_viewport)
{
	jgl::Vector2 pos = tile_to_screen(player_pos, _pos);
	jgl::Vector2 dir_delta = jgl::Vector2(static_cast<int>(_look_dir), 0);
	jgl::Vector2 delta = 0;
	size_t value = 0;

	
	if (is_moving() == true)
		value += tmp_delta[static_cast<size_t>(_look_dir)][jgl::get_frame_state(tmp_delta[static_cast<size_t>(_look_dir)].size())];
	else if (_look_dir == Entity_direction::north || _look_dir == Entity_direction::south)
		value += 1;
	delta.x += value;
	_charset->draw(_sprite + delta + dir_delta, pos, node_size, 1.0f, p_viewport);
}