#include "erelia.h"

Interaction::Interaction()
{
	_type = Interaction_type::none;
	_param.item_id = 0;
	_param.next_interaction = -1;
}

Interaction::Interaction(Interaction_type p_type)
{
	_type = p_type;
	_param.item_id = 0;
	_param.next_interaction = -1;
}

void Interaction::read_action_dialogue(jgl::Array<jgl::String> tab, size_t index)
{
	_param.text = tab[index];
	_param.next_interaction = jgl::stoi(tab[index + 1]);
}

void Interaction::read_action_tp(jgl::Array<jgl::String> tab, size_t index)
{
	_param.tp_destination = jgl::Vector2(jgl::stoi(tab[index]), jgl::stoi(tab[index + 1]));
	_param.next_interaction = jgl::stoi(tab[index + 2]);
}

void Interaction::read_action_give_object(jgl::Array<jgl::String> tab, size_t index)
{
	_param.item_id = jgl::stoi(tab[index]);
	_param.next_interaction = jgl::stoi(tab[index + 1]);
}

void Interaction::read_action_move_both(jgl::Array<jgl::String> tab, size_t index)
{
	_param.delta_pos = jgl::Vector2(jgl::stoi(tab[index]), jgl::stoi(tab[index + 1]));
	_param.next_interaction = jgl::stoi(tab[index + 2]);
}

void Interaction::read_action_move_target(jgl::Array<jgl::String> tab, size_t index)
{
	_param.delta_pos = jgl::Vector2(jgl::stoi(tab[index]), jgl::stoi(tab[index + 1]));
	_param.next_interaction = jgl::stoi(tab[index + 2]);
}

void Interaction::read_action_move_source(jgl::Array<jgl::String> tab, size_t index)
{
	_param.delta_pos = jgl::Vector2(jgl::stoi(tab[index]), jgl::stoi(tab[index + 1]));
	_param.next_interaction = jgl::stoi(tab[index + 2]);
}

jgl::Array<I_funct> interact_parse_tab = {
	&Interaction::read_action_dialogue,
	&Interaction::read_action_tp,
	&Interaction::read_action_give_object,
	&Interaction::read_action_move_both,
	&Interaction::read_action_move_target,
	&Interaction::read_action_move_source,
};