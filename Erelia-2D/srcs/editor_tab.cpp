#include "erelia.h"

Editor_inventory_tab::Editor_inventory_tab(size_t p_nb_slot, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_nb_slot = p_nb_slot;
	_background = new jgl::Frame(this);
	_background->activate();
	_background->send_back();

	_scroll_bar = new jgl::Vscroll_bar(_background);
	_scroll_bar->activate();
}

Item_slot* Editor_inventory_tab::add_item_slot(Item* p_item)
{
	Item_slot* result = new Item_slot(p_item, _background);
	result->box().set_border(0);
	result->activate();
	_item_slots.push_back(result);
	return (result);
}

void Editor_inventory_tab::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_background->set_geometry(0, p_area);
	_scroll_bar->set_geometry(jgl::Vector2(p_area.x - 20, 0.0f), jgl::Vector2(20.0f, p_area.y));
	float size = (p_area.x - 40) / _nb_slot;

	for (size_t i = 0; i < _item_slots.size(); i++)
	{
		jgl::Vector2 pos = jgl::Vector2(10 + ((i % _nb_slot) * (size)), 10 + ((i / _nb_slot) * (size)));
		_item_slots[i]->set_geometry(pos, size);
	}
}

void Editor_inventory_tab::render()
{
	if (area() == -1)
		return;
}