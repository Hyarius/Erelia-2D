#include "erelia.h"

Battle_mode::Battle_mode(jgl::Widget* parent)
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
	return (false);
}

void Battle_mode::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}
void Battle_mode::render()
{
	//engine->board()->render(_viewport);
	if (_area != nullptr)
		_area->render(_viewport);
	//engine->player()->render(_viewport);
	jgl::draw_text("Gamemode : Battle", 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), jgl::Vector2(0, 20) + 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}