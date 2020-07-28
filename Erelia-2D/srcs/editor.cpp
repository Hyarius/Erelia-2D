#include "erelia.h"

extern jgl::Array<jgl::String> item_type_name;
extern jgl::Array<jgl::Array<Item*>*> item_array;

void activate_tab(jgl::Data param)
{
	Editor_inventory_tab* tab = param.acces< Editor_inventory_tab*>(0);
	Editor_inventory_tab** tab2 = param.acces< Editor_inventory_tab**>(1);

	if (tab == *tab2)
		return;

	if (tab != nullptr)
		tab->activate();
	if (*tab2 != nullptr)
		(*tab2)->desactivate();
	*tab2 = tab;
}

Editor_inventory::Editor_inventory(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_background = new jgl::Frame(this);

	_shortcut = new Editor_shortcut_bar(this);
	for (size_t i = 0; i < 8; i++)
		_shortcut->set_item(i, item_array[0]->operator[](i));
	_shortcut->activate();

	size_t nb_part = 5;
	size_t part = item_array[0]->size() / nb_part;
	for (size_t i = 0; i < item_type_name.size() + nb_part - 1; i++)
	{
		Editor_inventory_tab* result = new Editor_inventory_tab(19, _background);
		if (i < nb_part)
		{
			size_t delta = i * part;
			for (size_t k = 0; k < part; k++)
				result->add_item_slot(item_array[0]->operator[](k + delta));
		}
		else
		{
			for (size_t j = 0; j < item_array[i - nb_part + 1]->size(); j++)
				result->add_item_slot(item_array[i - nb_part + 1]->operator[](j));
		}
		_tab.push_back(result);
	}

	for (size_t i = 0; i < item_type_name.size() + nb_part - 1; i++)
	{
		jgl::Button* result = new jgl::Button(activate_tab, jgl::Data(2, _tab[i], &_active_tab), _background);
		if (i < nb_part)
			result->set_text(item_type_name[0]);
		else
			result->set_text(item_type_name[i - nb_part + 1]);
		result->activate();
		_tab_button.push_back(result);
	}

	activate_tab(jgl::Data(2, _tab[0], &_active_tab));
}

Item* Editor_inventory::select_item()
{
	Item_slot* selected = _shortcut->selected();

	if (selected == nullptr)
		return (nullptr);
	return (selected->item());
}

bool Editor_inventory::status()
{
	return (_activated);
}

void Editor_inventory::enable()
{
	_activated = true;
	_background->activate();
	_shortcut->set_frozen(true);
}

void Editor_inventory::disable()
{
	_activated = false;
	_background->desactivate();
	_shortcut->set_frozen(false);
}

void Editor_inventory::update()
{

}

bool Editor_inventory::handle_keyboard()
{
	return (false);
}

bool Editor_inventory::handle_mouse()
{
	return (false);
}

void Editor_inventory::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	float tmp_size = node_size * 2.5f + 5;
	jgl::Vector2 shortcut_size = jgl::Vector2(tmp_size * 10, tmp_size);
	jgl::Vector2 tab_size = jgl::Vector2(shortcut_size.x, p_area.y - shortcut_size.y - 30);
	float tmp_value = (p_area.x - tab_size.x - 40) / 2.0f;
	jgl::Vector2 button_size = jgl::Vector2(tmp_value > 100 ? 100 : tmp_value, 30.0f);
	jgl::Vector2 frame_size = jgl::Vector2(tab_size.x + button_size.x * 2 + 40, p_area.y - 10 - shortcut_size.y);

	jgl::Vector2 frame_pos = ((p_area - 10 - jgl::Vector2(10.0f, shortcut_size.y)) - frame_size) / 2;
	jgl::Vector2 tab_pos = 10;
	jgl::Vector2 button_pos = jgl::Vector2(frame_size.x - (button_size.x + 10) * 2, 10.0f);

	_background->set_geometry(frame_pos, frame_size);

	_shortcut->set_geometry(jgl::Vector2((p_area.x - shortcut_size.x) / 2, frame_size.y + 10.0f), shortcut_size);

	for (size_t i = 0; i < _tab.size(); i++)
		_tab[i]->set_geometry(10, tab_size);

	for (size_t i = 0; i < _tab_button.size(); i++)
		_tab_button[i]->set_geometry(button_pos + jgl::Vector2((i % 2) * (button_size.x + 10), (i / 2) * (button_size.y + 10)), button_size);
}

void Editor_inventory::render()
{
	if (area() == -1)
		return;
}