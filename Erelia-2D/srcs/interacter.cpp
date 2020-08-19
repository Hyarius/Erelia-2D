#include "erelia.h"

Interacter::Interacter(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_index = 0;
	_reset = false;
	_waiting = false;
	_source = nullptr;
	_target = nullptr;
	_dialogue_frame = new Dialogue_frame(this);
	_automatic = true;
}

void Interacter::run_action()
{
	if (_source->interaction().size() == 0 || _source->interaction(_index)->type() == Interaction_type::none)
		return;
	Interaction *tmp_interact = _source->interaction(_index);
	size_t tmp_index = static_cast<size_t>(tmp_interact->type());

	M_funct tmp = action_tab[tmp_index];
	(this->*tmp)(tmp_interact->param());
}

void Interacter::update()
{
	static size_t iteration = 0;
	if (_reset == true)
	{
		run_action();
		iteration++;
		if (iteration >= 10)
		{
			iteration = 0;
			increment();
		}

	}
	else if (_automatic == true)
	{
		if (_waiting == false || (_target->is_active() == false && _source->is_active() == false))
			increment();
	}
}

void Interacter::increment()
{
	if (_source == nullptr || _target == nullptr)
		return;

	_reset = false;
	_waiting = false;
	_automatic = false;
	_index++;
	_dialogue_frame->desactivate();
	if (_index >= _source->interaction().size())
	{
		if (_source->check_point().size() == 1)
			_source->return_starting_position();
		else 
			_source->calc_next_road();
		if (_target->check_point().size() == 1)
			_target->return_starting_position();
		else
			_target->calc_next_road();
		engine->desactive_interacter();
	}
	else
		run_action();
}

bool Interacter::handle_keyboard()
{
	if (jgl::get_key(jgl::key::space) == jgl::key_state::release && _waiting == false && _automatic == false && _reset == false)
	{
		increment();
		return (true);
	}
	return (false);
}

void Interacter::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	float offset = 10;
	jgl::Vector2 frame_size = jgl::Vector2(p_area.x - offset * 2, p_area.y / 4.0f);
	_dialogue_frame->set_geometry(jgl::Vector2(offset, p_area.y - frame_size.y - offset), frame_size);
}

void Interacter::render()
{

}