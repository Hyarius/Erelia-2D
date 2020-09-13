#include "erelia.h"

Adventure_mode::Adventure_mode(jgl::Widget* parent) : Game_mode(parent)
{

	_player_controller = new Player_controller(this);
	_player_controller->activate();

	_interacter = new Interacter(this);
}

bool Adventure_mode::handle_keyboard()
{
	return (false);
}

void Adventure_mode::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_interacter->set_geometry(0, p_area);
}

void Adventure_mode::render()
{
	engine->board()->render(_viewport);
	engine->player()->render(_viewport);
	jgl::draw_text("Gamemode : Adventure", 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), jgl::Vector2(0, 20) + 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}