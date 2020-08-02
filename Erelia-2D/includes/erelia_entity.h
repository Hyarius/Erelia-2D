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
private:
	Entity_direction _look_dir;
	jgl::Sprite_sheet* _charset;
	jgl::Vector2 _sprite;
	jgl::Vector2 _pos;
	jgl::Vector2 _direction;
	float _move_speed;

	bool _in_motion;
	float _total_tick;
	float _last_tick;
	float _actual_tick;
	float _move_tick;
public:
	Entity(jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite);
	void place(jgl::Vector2 p_pos);

	void set_move_speed(float p_state) { _move_speed = p_state; }
	float move_speed() { return (_move_speed); }

	void set_move_tick(float p_state) { _move_tick = p_state; }
	float move_tick() { return (_move_tick); }

	jgl::Vector2 pos() { return (_pos); }

	bool is_moving() { return (_in_motion); }
	bool is_active() { return (_direction != 0); }

	void render(jgl::Viewport* p_viewport);
	void move(jgl::Vector2 delta);
	void update();

	virtual void interact() = 0;
};

#endif