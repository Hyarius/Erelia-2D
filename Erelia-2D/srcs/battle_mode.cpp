#include "erelia.h"

Battle_mode::Battle_mode(jgl::Widget* parent)
{
	_contener = new jgl::Contener(this);
	_contener->activate();

	_pointer = new Entity(Entity_type::entity, "", 0, -1);
	_pointer->set_sprite(jgl::Vector2(2, 0));
	_pointer->set_tileset(engine->battle_tileset());
	_arena = nullptr;
}

void Battle_mode::start(Battle_area* p_arena)
{
	_arena = p_arena;
	_pointer->place(engine->player()->pos());
}

void Battle_mode::update()
{
	_pointer->update_pos(false);
}

bool Battle_mode::handle_keyboard()
{
	static jgl::key key_value[4] = { jgl::key::s, jgl::key::w, jgl::key::a, jgl::key::d };
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };

	if (_pointer->is_static() == true)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (jgl::get_key(key_value[i]) == jgl::key_state::down)
			{
				_pointer->move(move_delta[i], false);
				return (true);
			}
		}
	}
	else
		return (false);
	return (false);
}

void Battle_mode::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}
void Battle_mode::render()
{
	engine->board()->render(_viewport, _pointer->pos());
	if (_arena != nullptr)
		_arena->render(_viewport, _pointer->pos() - 0.5f);
	_pointer->render(_viewport, _pointer->pos());
	jgl::draw_text("Gamemode : Battle", 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), jgl::Vector2(0, 20) + 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}