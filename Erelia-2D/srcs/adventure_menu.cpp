#include "erelia.h"

Adventure_menu::Adventure_menu(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_selected_slot = nullptr;
	_frame = new jgl::Frame(this);
	_frame->activate();
	Team_comp* team = engine->player()->team();

	for (size_t i = 0; i < 6; i++)
	{
		_slots[i] = new Creature_slot(&(team->unit[i]), _frame);
		_slots[i]->activate();
	}
}

bool Adventure_menu::handle_keyboard()
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

bool Adventure_menu::handle_mouse()
{
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (_slots[i]->is_pointed() == true)
			{
				if (_selected_slot == nullptr)
				{
					_slots[i]->select();
					_selected_slot = _slots[i];
				}
				else
				{
					_selected_slot->unselect();
					_slots[i]->unselect();
					Creature_entity* tmp = *(_slots[i]->creature());
					*(_slots[i]->creature()) = *(_selected_slot->creature());
					*(_selected_slot->creature()) = tmp;
					_selected_slot = nullptr;
				}
				return (true);
			}
		}
	}
	return (false);
}

void Adventure_menu::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_frame->set_geometry(0, p_area);
	jgl::Vector2 size = jgl::Vector2(p_area.x - 20.0f, 80.0f);
	for (size_t i = 0; i < 6; i++)
	{
		_slots[i]->set_geometry(jgl::Vector2(0.0f, i * (size.y + 10)) + 10, size);
	}
}

void Adventure_menu::render()
{

}