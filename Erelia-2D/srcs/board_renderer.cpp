#include "erelia.h"

Board_renderer::Board_renderer(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{

}

void Board_renderer::rebake()
{
	engine->board()->rebake(_viewport);
}

void Board_renderer::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	rebake();
}

void Board_renderer::render()
{
	engine->board()->render(_viewport, engine->player()->pos());
	engine->player()->render(_viewport, engine->player()->pos());
}