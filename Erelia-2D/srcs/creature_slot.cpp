#include "erelia.h"

Creature_slot::Creature_slot(Creature_entity** p_creature, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_selected = false;
	_creature = p_creature;
	_box = jgl::w_box_component(this);
	_box.set_anchor(0);
	_box.set_border(4);
	_box.set_back(jgl::Color(133, 136, 140));
	_box.set_front(jgl::Color(170, 177, 189));
}
void Creature_slot::select()
{
	_box.set_border(10);
	_box.set_back(jgl::Color(196, 195, 94));
}
void Creature_slot::unselect()
{
	_box.set_border(4);
	_box.set_back(jgl::Color(133, 136, 140));
}
void Creature_slot::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_anchor(p_anchor);
	_box.set_area(p_area);
}
void Creature_slot::render()
{
	_box.render(_viewport);
	if (_creature != nullptr && (*_creature) != nullptr)
	{
		engine->faceset()->draw((*_creature)->species()->face(), _box.border(), _box.area() - _box.border() * 2, 1.0f, _viewport);
	}
}