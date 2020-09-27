#include "erelia.h"


Item_slot::Item_slot(Item* p_item, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_select = false;
	_box = jgl::w_box_component(this);
	_item = p_item;
}

void Item_slot::select()
{
	if (_select == true)
		return;

	_box.set_anchor(_box.anchor() - 4);
	_box.set_area(_box.area() + 8);
	_box.set_back(jgl::Color(171, 163, 9));
	_box.set_front(jgl::Color(230, 221, 62));
	_box.set_border(8);
	_select = true;
}

void Item_slot::unselect()
{
	if (_select == false)
		return;

	_box.set_anchor(_box.anchor() + 4);
	_box.set_area(area());
	_box.set_back(jgl::Color(90, 90, 90));
	_box.set_front(jgl::Color(120, 120, 120));
	_box.set_border(5);
	_select = false;
}

void Item_slot::move(jgl::Vector2 delta)
{
	_box.set_anchor(_box.anchor() + delta);
	_viewport->set_anchor(_viewport->anchor() + delta);
}

bool Item_slot::handle_keyboard()
{
	return (false);
}

bool Item_slot::handle_mouse()
{
	return (false);
}

void Item_slot::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_area(p_area);
	_box.set_anchor(p_anchor);
}

void Item_slot::render()
{
	if (is_active() == false)
		return;

	_box.render(_viewport);
	if (_item != nullptr)
	{
		_item->draw(_box.anchor() + _box.border(), _box.area() - _box.border() * 2, _viewport);
		jgl::draw_rectangle(_box.anchor(), _box.area(), 1, jgl::Color(0, 0, 0), nullptr);
		if (_item->item_type() == Item_type::tile && jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::down)
			jgl::draw_text(jgl::itoa((static_cast<Node_item*>(_item))->node_index()), _box.anchor() + _box.border() * 2, 12, 1, 1.0f, jgl::text_color::light_green);
	}
}