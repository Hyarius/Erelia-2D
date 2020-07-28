#ifndef ERELIA_ITEM_SLOT_H
#define ERELIA_ITEM_SLOT_H

#include "jgl.h"
#include "erelia_item.h"

class Item_slot : public jgl::Widget
{
private:
	bool _select;
	jgl::w_box_component _box;
	Item* _item;

public:
	Item_slot(Item* p_item, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_select = false;
		_box = jgl::w_box_component(this);
		_item = p_item;
	}
	void set_item(Item* p_item) { _item = p_item; }
	Item* item() { return (_item); }
	jgl::w_box_component& box() { return (_box); }

	void select()
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

	void unselect()
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

	void move(jgl::Vector2 delta)
	{
		_box.set_anchor(_box.anchor() + delta);
		_viewport->set_anchor(anchor() + delta);
	}

	bool handle_keyboard()
	{
		return (false);
	}

	bool handle_mouse()
	{
		if (get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
		{
			if (this->is_pointed() == true)
			{
				Node_item* tmp = static_cast<Node_item*>(_item);
				std::cout << "Node pointed : " << (_item == nullptr ? "null" : jgl::itoa(tmp->node_index())) << std::endl;
				return (true);
			}
		}
		return (false);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		_box.set_area(p_area);
		_box.set_anchor(p_anchor);
	}

	void render()
	{
		if (is_active() == false)
			return;

		_box.render(_viewport);
		if (_item != nullptr)
			_item->draw(_box.anchor() + _box.border(), _box.area() - _box.border() * 2, _viewport);
	}
};

#endif