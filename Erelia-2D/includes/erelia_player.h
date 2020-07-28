#ifndef ERELIA_PLAYER_H
#define ERELIA_PLAYER_H

#include "jgl.h"
#include "erelia_define.h"

class Player
{
private:
	jgl::Vector2 _pos;
	jgl::Vector2 _direction;
	float _move_speed;
	float distance;

	float _total_tick;
	float _last_tick;
	float _actual_tick;
	float _move_tick;
public:
	Player()
	{
		_pos = chunk_size / 2.0f;
		_direction = 0;
		_move_tick = 75;
		_move_speed = 1.0f;
	}
	void place(jgl::Vector2 p_pos) { _pos = jgl::Vector2((floor((p_pos.x * 2) + 0.5) / 2), (floor((p_pos.y * 2) + 0.5) / 2)); }
	
	void set_move_speed(float p_state) { _move_speed = p_state; }
	float move_speed() { return (_move_speed); }

	void set_move_tick(float p_state) { _move_tick = p_state; }
	float move_tick() { return (_move_tick); }
	jgl::Vector2 pos() { return (_pos); }
	bool is_active() { return (_direction != 0); }
	void move(jgl::Vector2 delta)
	{
		if (is_active() == true)
			return;

		_total_tick = 0;
		_last_tick = static_cast<float>(g_time);
		_actual_tick = _last_tick;
		_direction = delta / (_move_tick / _move_speed);
	}
	void update()
	{
		if (_direction == 0)
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
};

#endif