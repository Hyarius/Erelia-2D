#include "erelia.h"

Entity::Entity(Entity_type p_type, jgl::String p_name, jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite)
{
	_type = p_type;
	_name = p_name;
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
	Node* tmp = board->node((this->pos() + delta).round());
	
	if (tmp != nullptr && tmp->occuped() == false &&
		board->can_acces(this->pos(), delta) == true)
		return (true);
	return (false);
}

void Entity::place(Board* board, jgl::Vector2 p_pos)
{
	if (board->node(_pos.round()) != nullptr)
		board->node(_pos.round())->set_occupant(nullptr);
	_pos = p_pos.round();
	if (board->node(_pos.round()) != nullptr)
		board->node(_pos.round())->set_occupant(this);
	_did_tp = true;
	_in_motion = true;
	_total_tick = 0;
	_last_tick = static_cast<float>(g_time);
	_actual_tick = _last_tick;
}

void Entity::update(Board* board)
{
	update_pos(board);
}

void Entity::move(Board* board, jgl::Vector2 delta)
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
	_in_motion = true;
	_did_tp = false;
	_total_tick = 0;
	_last_tick = static_cast<float>(g_time);
	_actual_tick = _last_tick;
	Node* tmp1 = board->node(_pos + delta);
	if (tmp1 != nullptr)
		tmp1->set_occupant(this);
	_direction = delta / (_move_tick / _move_speed);
}
void Entity::update_pos(Board *board)
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
		_direction = 0;

	if (rpos != _pos.round())
	{
		Node* tmp1 = board->node(rpos);
		Node* tmp2 = board->node(_pos.round());
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

bool Entity::is_pointed(jgl::Vector2 player_pos)
{
	jgl::Vector2 pos = tile_to_screen(player_pos, _pos);

	if (jgl::is_middle(pos.x, g_mouse->pos.x, pos.x + node_size) == true &&
		jgl::is_middle(pos.y, g_mouse->pos.y, pos.y + node_size) == true)
		return (true);
	return (false);
}

void Entity::interact()
{
	std::cout << "Interact with an Entity" << std::endl;
}

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
	if (_type != Entity_type::Player && is_pointed(player_pos) == true)
		jgl::draw_centred_text(_name, pos + node_size / 2 - jgl::Vector2(0, node_size), 16, 1, jgl::text_color::white);
}