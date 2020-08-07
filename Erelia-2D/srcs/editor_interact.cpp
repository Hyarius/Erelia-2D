#include "erelia.h"

Editor_interact::Editor_interact(Editor_inventory* p_inventory, Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_inventory = p_inventory;
	_board = p_board;
	_player = p_player;
}

bool Editor_interact::handle_click()
{
	if (_state == false)
	{
		_first = screen_to_tile(_player->pos(), g_mouse->pos);
		_state = true;
	}
	else
	{
		_second = screen_to_tile(_player->pos(), g_mouse->pos);
		_state = false;
		return (true);
	}
	return (false);
}

void Editor_interact::handle_multi_pos(Item* tmp)
{
	if (tmp == nullptr)
		return;

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
}

void Editor_interact::handle_remove_multi_pos()
{
	jgl::Vector2 start = jgl::Vector2((_first.x < _second.x ? _first.x : _second.x), (_first.y < _second.y ? _first.y : _second.y));
	jgl::Vector2 end = jgl::Vector2((_first.x > _second.x ? _first.x : _second.x), (_first.y > _second.y ? _first.y : _second.y));
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			jgl::Vector2 pos = jgl::Vector2(i, j);
			_board->place(pos, nullptr);
		}
	start = (start / chunk_size).floor();
	end = (end / chunk_size).floor();
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			_board->bake_chunk(jgl::Vector2(i, j));
		}
}

extern jgl::Array<Item*> node_item_list;

bool Editor_interact::handle_mouse()
{
	Item* tmp = _inventory->select_item();

	if (jgl::get_button(jgl::mouse_button::center) == jgl::mouse_state::release)
	{
		jgl::Vector2 target = screen_to_tile(_player->pos(), g_mouse->pos);
		Tile* target_node = _board->tile(target);
		if (target_node == nullptr)
			return false;
		_inventory->selected_slot()->set_item(node_item_list[target_node->index]);
	}
	
	if (jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::pressed ||
		jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::release)
		{
			if (handle_click() == true)
				handle_remove_multi_pos();
			return (true);
		}

	if (tmp == nullptr)
		return (false);

	if (tmp->item_type() == Item_type::tile)
	{
		if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::pressed ||
			jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
			if (handle_click() == true)
				handle_multi_pos(tmp);
	}
	else if (tmp->item_type() == Item_type::interact)
	{
		if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
			tmp->use(jgl::Data(4, _board, _player, &_pink_flag, &_blue_flag));
	}
	else if (tmp->item_type() == Item_type::prefab)
	{
		if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
		{
			jgl::Vector2 target = screen_to_tile(_player->pos(), g_mouse->pos);
			tmp->use(jgl::Data(2, _board, &target));
		}
	}
	return (false);
}

void Editor_interact::update()
{
	Item* tmp = _inventory->select_item();

	if (tmp == nullptr)
		return;

	if (tmp->item_type() != Item_type::tile)
		_state = false;
}

void Editor_interact::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
}

void Editor_interact::render()
{
	Item* tmp = _inventory->select_item();

	if (tmp == nullptr)
		return;

	if (tmp->item_type() == Item_type::tile)
	{
		if (_state == true)
		{
			jgl::Vector2 _third = screen_to_tile(_player->pos(), g_mouse->pos);
			jgl::Vector2 start = jgl::Vector2((_first.x < _third.x ? _first.x : _third.x), (_first.y < _third.y ? _first.y : _third.y));
			jgl::Vector2 end = jgl::Vector2((_first.x > _third.x ? _first.x : _third.x), (_first.y > _third.y ? _first.y : _third.y));
			for (float i = start.x; i <= end.x; i++)
				for (float j = start.y; j <= end.y; j++)
				{
					jgl::Vector2 coord = tile_to_screen(_player->pos(), jgl::Vector2(i, j));
					jgl::draw_rectangle(coord, node_size, 1, jgl::Color(0, 0, 0), _viewport);
				}
		}
	}
	else if (tmp->item_type() == Item_type::interact)
	{
		jgl::draw_rectangle(tile_to_screen(_player->pos(), _pink_flag), node_size, 1, jgl::Color(250, 7, 100), _viewport);
		jgl::draw_rectangle(tile_to_screen(_player->pos(), _blue_flag), node_size, 1, jgl::Color(66, 135, 245), _viewport);
	}
	
}