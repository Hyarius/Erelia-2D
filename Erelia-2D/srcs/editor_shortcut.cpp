#include "erelia.h"

Editor_shortcut_bar::Editor_shortcut_bar(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_index = 0;
	_background = new jgl::Frame(this);
	_background->activate();
	for (size_t i = 0; i < 10; i++)
	{
		Item_slot* result = new Item_slot(nullptr, _background);
		result->activate();

		result->box().set_back(jgl::Color(90, 90, 90));
		result->box().set_front(jgl::Color(120, 120, 120));
		_slots.push_back(result);
	}
}
void Editor_shortcut_bar::set_item(size_t index, Item* p_item)
{
	if (index >= _slots.size())
		return;
	_slots[index]->set_item(p_item);
}

bool Editor_shortcut_bar::handle_mouse()
{
	if (g_mouse->wheel != 0)
	{
		if (_selected != nullptr)
			_selected->unselect();
		_index = _index + static_cast<int>(-g_mouse->wheel);
		while (_index < 0)
			_index += _slots.size();
		while (static_cast<size_t>(_index) >= _slots.size())
			_index -= _slots.size();
		_slots[_index]->select();
		_selected = _slots[_index];
	}
	return (false);
}

void Editor_shortcut_bar::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_background->set_geometry(0, p_area);

	size_t nb = _slots.size();
	float offset = 10;
	float size = p_area.y - offset * 2;
	float space = ((p_area.x - offset * 2) - (nb * size)) / (nb - 1);
	jgl::Vector2 delta = (p_area - jgl::Vector2(space * (nb - 1) + size * nb, size)) / 2;
	for (size_t i = 0; i < nb; i++)
	{
		jgl::Vector2 pos = delta + jgl::Vector2((size + space) * i, 0.0f);
		_slots[i]->set_geometry(pos, size);
	}
	_slots[_index]->select();
	_selected = _slots[_index];
}

void Editor_shortcut_bar::render()
{
	if (area() == -1)
		return;
}