#include "erelia.h"

Editor_interact::Editor_interact(Editor_inventory* p_inventory, Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_inventory = p_inventory;
	_board = p_board;
	_player = p_player;
}

void Editor_interact::handle_multi_pos()
{
	Item* tmp = _inventory->select_item();
	if (tmp == nullptr)
		return;

	if (_state == false)
	{
		_first = _board->tile_coord(_player->pos(), g_mouse->pos);
		_state = true;
	}
	else
	{
		_second = _board->tile_coord(_player->pos(), g_mouse->pos);
		jgl::Vector2 start = jgl::Vector2((_first.x < _second.x ? _first.x : _second.x), (_first.y < _second.y ? _first.y : _second.y));
		jgl::Vector2 end = jgl::Vector2((_first.x > _second.x ? _first.x : _second.x), (_first.y > _second.y ? _first.y : _second.y));
		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				jgl::Vector2 pos = jgl::Vector2(i, j);
				tmp->use(jgl::Data(2, _board, &pos));
			}
		start = (start / chunk_size).floor();
		end = (end / chunk_size).floor();
		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				_board->bake_chunk(jgl::Vector2(i, j));
			}

		_state = false;
	}
}

bool Editor_interact::handle_mouse()
{
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::pressed ||
		jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
		handle_multi_pos();
	return (false);
}

void Editor_interact::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}

void Editor_interact::render()
{
	if (_state == true)
	{
		jgl::Vector2 _third = _board->tile_coord(_player->pos(), g_mouse->pos);
		jgl::Vector2 start = jgl::Vector2((_first.x < _third.x ? _first.x : _third.x), (_first.y < _third.y ? _first.y : _third.y));
		jgl::Vector2 end = jgl::Vector2((_first.x > _third.x ? _first.x : _third.x), (_first.y > _third.y ? _first.y : _third.y));
		for (float i = start.x; i <= end.x; i++)
			for (float j = start.y; j <= end.y; j++)
			{
				jgl::Vector2 coord = _board->tile_on_screen(_player->pos(), jgl::Vector2(i, j));
				jgl::draw_rectangle(coord, node_size, 1, jgl::Color(0, 0, 0), _viewport);
			}
	}
}