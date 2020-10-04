#include "erelia.h"

Editor_mode::Editor_mode(jgl::Widget* parent) : Game_mode(parent)
{
	_contener = new jgl::Contener(this);
	_contener->activate();

	_inventory = new Editor_inventory(_contener);
	_inventory->activate();

	_editor_interacter = new Editor_interact(_contener);
	_editor_interacter->activate();
	_inventory->send_front();

	_player_controller = new Player_controller(_contener);
	_player_controller->activate();

	_interacter = new Interacter(_contener);

	_board_renderer = new Board_renderer(_contener);
	_board_renderer->activate();
}

void Editor_mode::on_activation()
{
	std::cout << "Here ?" << std::endl;
	_board_renderer->rebake();
}

void Editor_mode::interact_between(Entity* source, Entity* target)
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

void Editor_mode::active_interacter()
{
	_contener->desactivate();
	_player_controller->set_frozen(true);
	_interacter->activate();
	_interacter->run_action();
}

void Editor_mode::desactive_interacter()
{
	_contener->activate();
	_player_controller->set_frozen(false);
	_interacter->activate();
	_interacter->set_entity(nullptr, nullptr);
}

void Editor_mode::active_inventory()
{
	_player_controller->set_frozen(true);
	_editor_interacter->desactivate();
	_inventory->enable();
}

void Editor_mode::desactive_inventory()
{
	_player_controller->set_frozen(false);
	_editor_interacter->activate();
	_inventory->disable();
}

bool Editor_mode::handle_keyboard()
{
	if (jgl::get_key(jgl::key::tab) == jgl::key_state::release || (_inventory->status() == true && jgl::get_key(jgl::key::escape) == jgl::key_state::release))
	{
		if (_contener->is_frozen() == false)
		{
			if (_inventory->status() == false)
				active_inventory();
			else
				desactive_inventory();
			return (true);
		}
	}

	return (false);
}

void Editor_mode::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	float tmp = g_application->size().x / 30;
	_inventory->set_geometry(tmp, p_area - tmp * 2);
	_editor_interacter->set_geometry(0, p_area);
	_interacter->set_geometry(0, p_area);
	_board_renderer->set_geometry(0, p_area);
}

void Editor_mode::render()
{
	jgl::draw_text("Gamemode : Editor", 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), jgl::Vector2(0, 20) + 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}