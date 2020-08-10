#ifndef ERELIA_ENTITY_H
#define ERELIA_ENTITY_H

enum class Entity_direction
{
	south = 0,
	north = 3,
	west = 6,
	east = 8
};

class Entity
{
protected:
	jgl::String _name;
	Entity_direction _look_dir;
	jgl::Sprite_sheet* _charset;
	jgl::Vector2 _sprite;
	jgl::Vector2 _pos;
	jgl::Vector2 _direction;
	float _move_speed;

	bool _did_tp;
	bool _in_motion;
	Uint32 _wait_time;
	float _total_tick;
	float _last_tick;
	float _actual_tick;
	float _move_tick;
public:
	Entity(jgl::String p_name, jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite);
	jgl::String name() { return (_name); }
	void set_name(jgl::String p_name) { _name = p_name; }
	void place(class Board* board, jgl::Vector2 p_pos);

	void set_wait_time(Uint32 p_time) { _wait_time = p_time; }
	Uint32 wait_time() { return (_wait_time); }

	void set_move_speed(float p_state) { _move_speed = p_state; }
	float move_speed() { return (_move_speed); }

	void set_move_tick(float p_state) { _move_tick = p_state; }
	float move_tick() { return (_move_tick); }

	void set_look_dir(Entity_direction p_look_dir) { _look_dir = p_look_dir; }
	Entity_direction look_dir() { return (_look_dir); }

	float last_tick() { return (_last_tick); }

	bool can_move(class Board* board, jgl::Vector2 delta);
	jgl::Vector2 direction() { return (_direction); }
	jgl::Vector2 pos() { return (_pos); }

	bool did_tp() { return (_did_tp); }
	bool is_static() { return (_total_tick >= (_move_tick / _move_speed)); }
	bool is_moving() { return (_in_motion); }
	bool is_active() { return (_direction != 0); }

	void render(jgl::Vector2 player_pos, jgl::Viewport* p_viewport);
	void move(jgl::Vector2 delta);
	virtual void update(class Board* board);
	void update_pos(class Board* board);
};

#endif