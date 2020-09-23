#include "erelia.h"

Adventure_mode::Adventure_mode(jgl::Widget* parent) : Game_mode(parent)
{

	_player_controller = new Player_controller(this);
	_player_controller->activate();

	_interacter = new Interacter(this);
	_interacter->activate();
}

void Adventure_mode::interact_between(Entity* source, Entity* target)
{
	if (source == nullptr || target == nullptr)
		return;
	if (target->look_dir() == Entity_direction::south)
		source->set_look_dir(Entity_direction::north);
	else if (target->look_dir() == Entity_direction::north)
		source->set_look_dir(Entity_direction::south);
	else if (target->look_dir() == Entity_direction::east)
		source->set_look_dir(Entity_direction::west);
	else if (target->look_dir() == Entity_direction::west)
		source->set_look_dir(Entity_direction::east);

	if (source->interaction().size() == 0)
		return;

	_interacter->set_entity(source, target);

	active_interacter();
}

void Adventure_mode::active_interacter()
{
	_player_controller->set_frozen(true);
	_interacter->activate();
	_interacter->run_action();
}

void Adventure_mode::desactive_interacter()
{
	_player_controller->set_frozen(false);
	_interacter->activate();
	_interacter->set_entity(nullptr, nullptr);
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