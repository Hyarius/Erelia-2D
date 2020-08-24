#ifndef ERELIA_ENTITY_H
#define ERELIA_ENTITY_H

#include "erelia_interaction.h"

enum class Entity_type
{
	entity = 0,
	NPC = 1,
	Player = 2,
};

enum class Entity_direction
{
	south = 0,
	north = 3,
	west = 6,
	east = 8
};

enum class Entity_movement
{
	controled = 1,
	errant = -1,
	fix = 0
};

class Entity
{
protected:
	Entity_type _type;
	jgl::String _name;
	Entity_direction _look_dir;
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

	Entity_movement _movement_type;
	int _movement_range;
	jgl::Vector2 _starting_pos;
	jgl::Array<jgl::Vector2> _check_point;
	size_t _check_point_index;
	jgl::Array<jgl::Vector2> _road;
	size_t _road_index;

	std::map<int, Interaction> _interaction;

public:
	Entity();
	Entity(Entity_type p_type, jgl::String p_name, jgl::Vector2 p_pos, jgl::Vector2 p_sprite);
	jgl::String name() { return (_name); }
	void set_name(jgl::String p_name) { _name = p_name; }
	void place(jgl::Vector2 p_pos);

	void add_interaction(int index, Interaction p_interaction) { _interaction[index] = p_interaction; }
	std::map<int, Interaction>& interaction() { return (_interaction); }
	Interaction interaction(int index) { if (_interaction.count(index) == 0)return (Interaction(Interaction_type::none)); return (_interaction[index]); }

	jgl::Vector2 sprite() { return (_sprite); }
	Entity_type type(){ return (_type); }

	void set_wait_time(Uint32 p_time) { _wait_time = p_time; }
	Uint32 wait_time() { return (_wait_time); }

	void set_move_speed(float p_state) { _move_speed = p_state; }
	float move_speed() { return (_move_speed); }

	void set_move_tick(float p_state) { _move_tick = p_state; }
	float move_tick() { return (_move_tick); }

	void set_look_dir(Entity_direction p_look_dir) { _look_dir = p_look_dir; }
	Entity_direction look_dir() { return (_look_dir); }

	float last_tick() { return (_last_tick); }
	jgl::Vector2 direction() { return (_direction); }
	jgl::Vector2 pos() { return (_pos); }

	bool did_tp() { return (_did_tp); }
	bool is_static() { return (_total_tick >= (_move_tick / _move_speed)); }
	bool is_moving() { return (_in_motion); }
	bool is_active() { return (_direction != 0); }
	bool is_interacting();

	bool is_pointed(jgl::Vector2 target);

	void render(jgl::Viewport* p_viewport);
	void move(jgl::Vector2 delta);
	void update_pos();

	bool can_move(jgl::Vector2 delta);

	virtual void update();

	void remove_check_point(jgl::Vector2 pos);
	void add_check_point(jgl::Vector2 p_point) { _check_point.push_back(p_point); }
	void calc_next_road();
	void calc_road_to(jgl::Vector2 destination);
	void return_starting_position();

	void set_movement_type(Entity_movement p_mvt)
	{
		_movement_type = p_mvt;
		calc_road_to(_starting_pos);
	}
	void set_movement_range(int p_range) { _movement_range = p_range; }

	Entity_movement movement_type() { return (_movement_type); }
	int movement_range() { return (_movement_range); }

	void set_starting_pos(jgl::Vector2 p_pos) { _starting_pos = p_pos; }
	jgl::Vector2 starting_pos() { return (_starting_pos); }
	jgl::Array<jgl::Vector2>& check_point() { return (_check_point); }
	jgl::Vector2 check_point(size_t i) { return (_check_point[i]); }
	jgl::Array<jgl::Vector2>& road() { return (_road); }
	jgl::Vector2 road(size_t i) { return (_road[i]); }
};

#endif