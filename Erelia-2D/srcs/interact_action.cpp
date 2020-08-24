#include "erelia.h"

int Interacter::action_dialogue(Interaction_data param)
{
	_dialogue_frame->activate();
	_dialogue_frame->set_text(param.text);
	_automatic = false;
	_waiting = false;
	return (param.next_interaction);
}

int Interacter::action_tp(Interaction_data param)
{
	_target->place(param.tp_destination);
	_automatic = true;
	_waiting = false;
	return (param.next_interaction);
}

int Interacter::action_move_both(Interaction_data param)
{
	jgl::Vector2 delta = param.delta_pos;
	if (_target->can_move(delta) && _source->can_move(delta) == true)
	{
		_target->move(delta);
		_source->move(delta);
		_reset = false;
	}
	else
		_reset = true;
	_automatic = true;
	_waiting = true;
	return (param.next_interaction);
}

int Interacter::action_move_target(Interaction_data param)
{
	jgl::Vector2 delta = param.delta_pos;
	if (_target->can_move(delta))
	{
		_target->move(delta);
		_reset = false;
	}
	else
		_reset = true;
	_automatic = true;
	_waiting = true;
	return (param.next_interaction);
}

int Interacter::action_move_source(Interaction_data param)
{
	jgl::Vector2 delta = param.delta_pos;
	if (_source->can_move(delta))
	{
		_source->move(delta);
		_reset = false;
	}
	else
		_reset = true;
	_automatic = true;
	_waiting = true;
	return (param.next_interaction);
}

int Interacter::action_give_object(Interaction_data param)
{
	_automatic = true;
	_waiting = false;
	return (param.next_interaction);
}

jgl::Array<M_funct> action_tab =
{
	&Interacter::action_dialogue,
	&Interacter::action_tp,
	&Interacter::action_give_object,
	&Interacter::action_move_both,
	&Interacter::action_move_target,
	&Interacter::action_move_source,
};

