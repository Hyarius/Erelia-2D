#include "erelia.h"

Battle_change_menu::Battle_change_menu(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_selected_slot = nullptr;
	Team_comp* team = engine->player()->team();

	for (size_t i = 0; i < 6; i++)
	{
		_slots[i] = new Creature_battle_slot(&(team->unit[i]), this);
		_slots[i]->activate();
	}
}

bool Battle_change_menu::handle_keyboard()
{
	if (jgl::get_key(jgl::key::escape) == jgl::key_state::release)
	{
		if (_selected_slot != nullptr)
		{
			_selected_slot->unselect();
			_selected_slot = nullptr;
			return (true);
		}
	}
	return (false);
}

bool Battle_change_menu::handle_mouse()
{
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (_slots[i]->is_pointed() == true)
			{
				if (_selected_slot != nullptr)
					_selected_slot->unselect();
				_slots[i]->select();
				_selected_slot = _slots[i];
				return (true);
			}
		}
	}
	return (false);
}

void Battle_change_menu::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	float delta = p_area.x / 4;
	jgl::Vector2 size = jgl::Vector2(p_area.x - 20.0f, (p_area.y - 25) / 6);
	for (size_t i = 0; i < 6; i++)
	{
		_slots[i]->set_geometry(jgl::Vector2(0.0f, i * (size.y + 5)), size);
	}
}

void Battle_change_menu::render()
{

}