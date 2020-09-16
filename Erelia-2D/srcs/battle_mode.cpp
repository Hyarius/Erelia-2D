#include "erelia.h"

Battle_mode::Battle_mode(jgl::Widget* parent)
{
	_contener = new jgl::Contener(this);
	_contener->activate();

	_pointer = new Entity(Entity_type::entity, "", 0, -1);
	_pointer->set_sprite(jgl::Vector2(static_cast<int>(Battle_node_type::select), 0));
	_pointer->set_tileset(engine->battle_tileset());
	_arena = nullptr;
}

uint32_t tmp_time;

void Battle_mode::start(Battle_area* p_arena)
{
	_arena = p_arena;
	_pointer->place(engine->player()->pos());
	tmp_time = g_time + 5000;
}

void Battle_mode::exit()
{
	engine->player()->place(_pointer->pos());
	engine->change_mode(game_mode::adventure);
	delete _arena;
	_arena = nullptr;
}

void Battle_mode::update()
{
	_pointer->update_pos(false);
}

bool Battle_mode::handle_keyboard()
{
	static jgl::key key_value[4] = { jgl::key::s, jgl::key::w, jgl::key::a, jgl::key::d };
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };

	
	for (size_t i = 0; i < 4; i++)
	{
		if (jgl::get_key(key_value[i]) == jgl::key_state::down)
		{
			jgl::Vector2 actual_pos = _pointer->pos() - _arena->pos() + move_delta[i];
			if (_pointer->is_static() == true && _arena->content().count(actual_pos) != 0 &&
				_arena->content()[actual_pos]->type != Battle_node_type::border)// &&
				// _arena->content()[actual_pos]->type != Battle_node_type::obstacle)
			{
				_pointer->move(move_delta[i], false);
				return (true);
			}
		}
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::release)
	{
		exit();
		return (true);
	}
	else
		return (false);
}

void Battle_mode::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}
void Battle_mode::render()
{
	engine->board()->render(_viewport, _pointer->pos());
	if (_arena != nullptr)
	{
		_arena->render(_viewport, _pointer->pos() - 0.5f);
		jgl::Vector2 start_pos = tile_to_screen(_arena->pos() - 1, _pointer->pos());
		jgl::Vector2 end_pos = tile_to_screen(_arena->pos() + _arena->size() + 1, _pointer->pos());
		jgl::fill_rectangle(0, jgl::Vector2(g_application->size().x, start_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(0.0f, end_pos.y) , jgl::Vector2(g_application->size().x, g_application->size().y - end_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(0.0f, start_pos.y), jgl::Vector2(start_pos.x, end_pos.y - start_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(end_pos.x, start_pos.y), jgl::Vector2(g_application->size().x - end_pos.x, end_pos.y - start_pos.y), jgl::Color(0, 0, 0, 200));
	}
	_pointer->render(_viewport, _pointer->pos());
	jgl::draw_text("Gamemode : Battle", 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), jgl::Vector2(0, 20) + 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}