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
	Entity_movement _movement_type;
	jgl::String _name;
	Entity_direction _look_dir;
	jgl::Vector2 _sprite;
	jgl::Vector2 _pos;
	jgl::Vector2 _direction;
	float _move_speed;

	jgl::Array<Interaction> _interaction;

	jgl::Vector2 _starting_pos;
	jgl::Array<jgl::Vector2> _check_point;
	size_t _check_point_index;
	jgl::Array<jgl::Vector2> _road;
	size_t _road_index;

	bool _did_tp;
	bool _in_motion;
	Uint32 _wait_time;
	float _total_tick;
	float _last_tick;
	float _actual_tick;
	float _move_tick;
public:
	Entity();
	Entity(Entity_type p_type, jgl::String p_name, jgl::Vector2 p_pos, jgl::Vector2 p_sprite);
	jgl::Array<Interaction>& interaction() { return (_interaction); }
	Interaction interaction(size_t index) { return (_interaction[index]); }
	jgl::String name() { return (_name); }
	void set_name(jgl::String p_name) { _name = p_name; }
	void place(jgl::Vector2 p_pos);

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

	bool can_move(jgl::Vector2 delta);
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
	virtual void update();
	void update_pos();

	void load(std::fstream& file);
	size_t load_from_line(jgl::Array<jgl::String> tab);
	void save(std::fstream& file);

	void remove_check_point(jgl::Vector2 pos);
	void add_check_point(jgl::Vector2 p_point) { _check_point.push_back(p_point); }
	void calc_next_road();
	void calc_road_to(jgl::Vector2 destination);
	void return_starting_position();

	jgl::Vector2 starting_pos() { return (_starting_pos); }
	jgl::Array<jgl::Vector2>& check_point() { return (_check_point); }
	jgl::Vector2 check_point(size_t i) { return (_check_point[i]); }
	jgl::Array<jgl::Vector2>& road() { return (_road); }
	jgl::Vector2 road(size_t i) { return (_road[i]); }
};

#endif