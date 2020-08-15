#include "erelia.h"

Interacter::Interacter(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_entity = nullptr;
	_dialogue_frame = new jgl::Frame(this);
	_dialogue_frame->activate();
}

void Interacter::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	float offset = 10;
	jgl::Vector2 frame_size = jgl::Vector2(p_area.x - offset * 2, p_area.y / 6.0f);
	_dialogue_frame->set_geometry(jgl::Vector2(offset, p_area.y - frame_size.y - offset), frame_size);
}

void Interacter::render()
{

}