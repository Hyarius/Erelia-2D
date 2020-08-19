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
	return (false);
}

void Renderer::render()
{

}

