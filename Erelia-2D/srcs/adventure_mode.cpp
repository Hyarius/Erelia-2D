#include "erelia.h"

Adventure_mode::Adventure_mode(jgl::Widget* parent) : Game_mode(parent)
{
	_contener = new jgl::Contener(this);
	_contener->activate();

	_player_controller = new Player_controller(_contener);
	_player_controller->activate();

	_interacter = new Interacter(_contener);
	_interacter->activate();

	_board_renderer = new Board_renderer(_contener);
	_board_renderer->activate();

	_adventure_menu = new Adventure_menu(_contener);
	_adventure_menu->activate();
}

void Adventure_mode::on_activation()
{
	_board_renderer->rebake();
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

	float delta = g_application->size().x / 4;
	_board_renderer->set_geometry(jgl::Vector2(delta, 0.0f), jgl::Vector2(g_application->size().x - delta, g_application->size().y));
	_adventure_menu->set_geometry(0, jgl::Vector2(delta, p_area.y));
}

void Adventure_mode::render()
{
	jgl::draw_text("Gamemode : Adventure", 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), jgl::Vector2(0, 20) + 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}