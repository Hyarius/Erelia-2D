#include "erelia.h"

Editor_mouse_indicator::Editor_mouse_indicator(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_item = nullptr;
}
void Editor_mouse_indicator::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}
void Editor_mouse_indicator::render()
{
	if (_item != nullptr)
	{
		g_application->viewport()->use();
		_item->draw(g_mouse->pos - node_size * 1.25f, node_size * 2.5f, g_application->viewport());
	}
}