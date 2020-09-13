#include "erelia.h"

Editor_interact::Editor_interact(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_selected_entity = nullptr;
}

bool Editor_interact::handle_click()
{
	if (_state == false)
	{
		_first = screen_to_tile(g_mouse->pos);
		_state = true;
	}
	else
	{
		_second = screen_to_tile(g_mouse->pos);
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
			tmp->use(jgl::Data(2, engine->board(), &pos));
		}
	start = (start / CHUNK_SIZE).floor();
	end = (end / CHUNK_SIZE).floor();
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			engine->board()->bake_chunk(jgl::Vector2(i, j));
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
			engine->board()->place(pos, nullptr);
		}
	start = (start / CHUNK_SIZE).floor();
	end = (end / CHUNK_SIZE).floor();
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			engine->board()->bake_chunk(jgl::Vector2(i, j));
		}
}

extern jgl::Array<Item*> node_item_list;

bool Editor_interact::handle_mouse()
{
	Item* tmp = engine->editor_mode()->inventory()->select_item();

	if (jgl::get_button(jgl::mouse_button::center) == jgl::mouse_state::release)
	{
		jgl::Vector2 target = screen_to_tile(g_mouse->pos);
		Tile* target_node = engine->board()->tile(target);
		if (target_node == nullptr)
			return false;
		engine->editor_mode()->inventory()->selected_slot()->set_item(node_item_list[target_node->index]);
	}
	
	if (jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::pressed ||
		jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::release)
		{
			if (tmp->item_type() == Item_type::tile && handle_click() == true)
				handle_remove_multi_pos();
			else if (tmp == interact_item_list[3] && _selected_entity != nullptr)
				_selected_entity->remove_check_point(screen_to_tile(g_mouse->pos));
			else if (tmp->item_type() == Item_type::npc && engine->board()->node(screen_to_tile(g_mouse->pos))->occupant() != nullptr)
			{
				if (engine->board()->node(screen_to_tile(g_mouse->pos))->occupant()->type() == Entity_type::NPC)
				{
					engine->board()->remove_npc(engine->board()->node(screen_to_tile(g_mouse->pos))->occupant());
				}
			}
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
			tmp->use(nullptr);
	}
	else if (tmp->item_type() == Item_type::prefab || tmp->item_type() == Item_type::npc)
	{
		if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
		{
			jgl::Vector2 target = screen_to_tile(g_mouse->pos);
			tmp->use(&target);
		}
	}
	return (false);
}

void Editor_interact::update()
{
	Item* tmp = engine->editor_mode()->inventory()->select_item();

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
	Item* tmp = engine->editor_mode()->inventory()->select_item();

	if (tmp == nullptr)
		return;

	if (tmp->item_type() == Item_type::tile)
	{
		if (_state == true)
		{
			jgl::Vector2 _third = screen_to_tile(g_mouse->pos);
			jgl::Vector2 start = tile_to_screen(jgl::Vector2((_first.x < _third.x ? _first.x : _third.x), (_first.y < _third.y ? _first.y : _third.y)));
			jgl::Vector2 end = tile_to_screen(jgl::Vector2((_first.x > _third.x ? _first.x : _third.x), (_first.y > _third.y ? _first.y : _third.y)) + 1);
			
			jgl::draw_rectangle(start, end - start, 1, jgl::Color(0, 0, 0), _viewport);
		}
	}
	else if (tmp->item_type() == Item_type::interact)
	{
		jgl::draw_rectangle(tile_to_screen(_pink_flag), node_size, 1, jgl::Color(250, 7, 100), _viewport);
		jgl::draw_rectangle(tile_to_screen(_blue_flag), node_size, 1, jgl::Color(66, 135, 245), _viewport);
	}

	if (_selected_entity != nullptr)
	{
		jgl::draw_centred_text(_selected_entity->name(), tile_to_screen(_selected_entity->pos()) + node_size / 2 - jgl::Vector2(0, node_size), 16, 1, 1.0f, jgl::text_color::green, jgl::text_style::normal);
		if (_selected_entity->check_point().size() > 1)
		{
			for (size_t i = 0; i < _selected_entity->check_point().size(); i++)
			{
				jgl::Vector2 pos = tile_to_screen(_selected_entity->check_point(i));
				jgl::draw_centred_text(jgl::itoa(i), pos + node_size / 2 - jgl::Vector2(0, node_size), 16, 1, 1.0f, jgl::text_color::green, jgl::text_style::normal);
				jgl::draw_rectangle(pos, node_size, 1, jgl::Color(0, 255, 0));
			}
		}
	}
	
}