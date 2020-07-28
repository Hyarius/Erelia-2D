#ifndef ERELIA_PLAYER_H
#define ERELIA_PLAYER_H

#include "jgl.h"
#include "erelia_define.h"

class Player
{
private:
	jgl::Vector2 _pos;
	jgl::Vector2 _direction;
	float distance;

	Uint32 _total_tick;
	Uint32 _last_tick;
	Uint32 _actual_tick;
	Uint32 _move_speed;
public:
	Player()
	{
		_pos = chunk_size / 2.0f;
		_direction = 0;
		_move_speed = 75;
	}
	void set_move_speed(Uint32 p_state) { _move_speed = p_state; }
	Uint32 move_speed() { return (_move_speed); }
	jgl::Vector2 pos() { return (_pos); }
	bool is_active() { return (_direction != 0); }
	void move(jgl::Vector2 delta)
	{
		if (is_active() == true)
			return;

		_total_tick = 0;
		_last_tick = g_time;
		_actual_tick = g_time;
		_direction = delta / _move_speed;
	}
	void update()
	{
		if (_direction == 0)
			return;

		_actual_tick = g_time;
		Uint32 delta = _actual_tick - _last_tick;

		if (_total_tick + delta > _move_speed)
		{
			delta = _move_speed - _total_tick;
		}
		_total_tick += delta;
		_last_tick = _actual_tick;
		jgl::Vector2 delta_pos = _direction * delta;
		_pos += delta_pos;
		if (_total_tick >= _move_speed)
			_direction = 0;
	}
};

#endif