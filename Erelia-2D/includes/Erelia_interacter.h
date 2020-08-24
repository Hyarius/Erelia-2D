#ifndef ERELIA_INTERACTER_H
#define ERELIA_INTERACTER_H

#include "jgl.h"

class Dialogue_frame : public jgl::Frame
{
private:
	jgl::String _text;
	jgl::Array<jgl::String> _tokens;
public:
	Dialogue_frame(jgl::Widget* p_parent);

	void set_text(jgl::String p_text);

	void render();
};

class Interacter : public jgl::Widget
{
private:
	bool _automatic;
	bool _waiting;
	bool _reset;
	Entity* _source;
	Entity* _target;

	int _index;

	Dialogue_frame* _dialogue_frame;

public:
	Interacter(jgl::Widget* p_parent = nullptr);
	Entity* source() { return (_source); }
	Entity* target() { return (_target); }
	Dialogue_frame* dialogue_frame(){ return (_dialogue_frame); }

	void set_entity(Entity* p_source, Entity* p_target)
	{
		_source = p_source;
		_target = p_target;
		_index = 0;
	}

	int action_dialogue(Interaction_data param);
	int action_tp(Interaction_data param);
	int action_give_object(Interaction_data param);
	int action_move_both(Interaction_data param);
	int action_move_target(Interaction_data param);
	int action_move_source(Interaction_data param);

	void run_action();

	void increment();

	void update();

	bool handle_keyboard();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

typedef 			int (Interacter::* M_funct)(Interaction_data param);

extern jgl::Array<M_funct> action_tab;

#endif