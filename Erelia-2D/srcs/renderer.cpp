#include "erelia.h"

Renderer::Renderer(Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_board = p_board;
	_player = p_player;
}

void Renderer::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}

void Renderer::render()
{
	_board->render(_player->pos(), _viewport);
	_player->render(_player->pos(), _viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), 50, 16, 1, jgl::text_color::white, jgl::text_style::normal, _viewport);
}

