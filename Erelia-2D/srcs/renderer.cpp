#include "erelia.h"

Renderer::Renderer(Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_board = p_board;
	_player = p_player;
}

void Renderer::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}

bool tmp = false;

bool Renderer::handle_keyboard()
{
	if (jgl::get_key(jgl::key::F9) == jgl::key_state::release)
	{
		tmp = !tmp;
	}
	return (false);
}

void Renderer::render()
{
	_board->render(_player->pos(), _viewport);
	_player->render(_player->pos(), _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), 50, 16, 1, jgl::text_color::white, jgl::text_style::normal, _viewport);

	if (tmp == true)
	{
		jgl::Array<jgl::Vector2> path = _board->pathfinding(_player->pos().round(), screen_to_tile(_player->pos(), g_mouse->pos));

		for (size_t i = 0; i < path.size(); i++)
		{
			jgl::Vector2 pos = tile_to_screen(_player->pos(), path[i]);
			Node* tmp = _board->node(path[i]);
			jgl::draw_rectangle(pos, node_size, 1, jgl::Color(0, 0, 0));
			jgl::draw_text(jgl::itoa(tmp->s_cost()), pos + jgl::Vector2(0.5f, 0.0f) * node_size, 10, 0, jgl::text_color::red, jgl::text_style::normal, _viewport);
			jgl::draw_text(jgl::itoa(tmp->e_cost()), pos + jgl::Vector2(0.0f, 0.5f) * node_size, 10, 0, jgl::text_color::green, jgl::text_style::normal, _viewport);
			jgl::draw_text(jgl::itoa(tmp->t_cost()), pos + jgl::Vector2(0.5f, 0.5f) * node_size, 10, 0, jgl::text_color::blue, jgl::text_style::normal, _viewport);
		}
	}
	
}

