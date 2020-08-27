#include "erelia.h"

Editor_mode::Editor_mode(jgl::Widget* parent) : Game_mode(parent)
{
	_contener = new jgl::Contener(this);
	_contener->activate();

	_inventory = new Editor_inventory(_contener);
	_inventory->activate();

	_interacter = new Editor_interact(_contener);
	_interacter->activate();
	_inventory->send_front();
}

void Editor_mode::enable()
{
	_interacter->set_frozen(true);
	_inventory->enable();
}
void Editor_mode::disable()
{
	_interacter->set_frozen(false);
	_inventory->disable();
}

bool Editor_mode::handle_keyboard()
{
	if (jgl::get_key(jgl::key::tab) == jgl::key_state::release || (_inventory->status() == true && jgl::get_key(jgl::key::escape) == jgl::key_state::release))
	{
		if (_contener->is_frozen() == false)
		{
			if (_inventory->status() == false)
				engine->active_inventory();
			else
				engine->desactive_inventory();
		}
	}
	return (false);
}

void Editor_mode::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	float tmp = g_application->size().x / 30;
	_inventory->set_geometry(tmp, p_area - tmp * 2);
	_interacter->set_geometry(0, p_area);
}
void Editor_mode::render()
{

}