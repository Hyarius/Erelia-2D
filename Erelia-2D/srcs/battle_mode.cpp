#include "erelia.h"

Battle_mode::Battle_mode(jgl::Widget* parent = nullptr)
{
	_contener = new jgl::Contener(this);
	_contener->activate();

	_area = nullptr;
}

void Battle_mode::start(Battle_area* p_area)
{
	_area = p_area;
}

bool Battle_mode::handle_keyboard()
{

}

void Battle_mode::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}
void Battle_mode::render()
{

}