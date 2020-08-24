#ifndef ERELIA_INTERACTION_H
#define ERELIA_INTERACTION_H

enum class Interaction_type
{
	none = -1,
	dialogue = 0,
	tp = 1,
	give_object = 2,
	move_both = 3,
	move_target = 4,
	move_source = 5,
	count
};

struct Interaction_data
{
	jgl::Vector2 tp_destination;
	jgl::Vector2 delta_pos;
	jgl::String text;
	size_t item_id;

	int next_interaction;
};

class Interaction
{
private:
	int index;
	Interaction_type _type;
	Interaction_data _param;

public:
	Interaction();
	Interaction(Interaction_type p_type);

	void read_action_dialogue(jgl::Array<jgl::String> tab, size_t index);
	void read_action_tp(jgl::Array<jgl::String> tab, size_t index);
	void read_action_give_object(jgl::Array<jgl::String> tab, size_t index);
	void read_action_move_both(jgl::Array<jgl::String> tab, size_t index);
	void read_action_move_target(jgl::Array<jgl::String> tab, size_t index);
	void read_action_move_source(jgl::Array<jgl::String> tab, size_t index);

	Interaction_data param() { return (_param); }
	Interaction_type type() { return (_type); }
};

typedef 			void (Interaction::* I_funct)(jgl::Array<jgl::String> tab, size_t index);

extern jgl::Array<I_funct> interact_parse_tab;

#endif