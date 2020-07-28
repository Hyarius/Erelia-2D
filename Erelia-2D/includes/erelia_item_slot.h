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
	Item_slot(Item* p_item, jgl::Widget* p_parent);
	void set_item(Item* p_item) { _item = p_item; }
	Item* item() { return (_item); }
	jgl::w_box_component& box() { return (_box); }

	void select();
	void unselect();
	void move(jgl::Vector2 delta);
	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif