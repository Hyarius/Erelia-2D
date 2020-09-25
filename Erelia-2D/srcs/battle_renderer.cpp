#include "erelia.h"

Battle_renderer::Battle_renderer(jgl::Widget* parent) : jgl::Widget(parent)
{
	_pointer = nullptr;
	_arena = nullptr;
	_turn_order = nullptr;
	_turn_index = nullptr;
}

void Battle_renderer::set_arena(Battle_arena* p_arena)
{
	_arena = p_arena;
}

void Battle_renderer::set_turn_order(jgl::Array<Creature_entity*>* p_array)
{
	_turn_order = p_array;
}

void Battle_renderer::set_pointer(Entity* p_pointer)
{
	_pointer = p_pointer;
}

void Battle_renderer::set_turn_index(int* p_turn_index)
{
	_turn_index = p_turn_index;
}

void Battle_renderer::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}
void Battle_renderer::render()
{
	if (_arena == nullptr || _pointer == nullptr || _turn_order == nullptr || _turn_index == nullptr)
		return;

	engine->board()->render(_viewport, _pointer->pos());
	if (_arena != nullptr)
	{
		_arena->render(_viewport, _pointer->pos() - 0.5f);
		jgl::Vector2 start_pos = engine->board()->tile_to_screen(_arena->pos() - 1, _pointer->pos());
		jgl::Vector2 end_pos = engine->board()->tile_to_screen(_arena->pos() + _arena->size() + 1, _pointer->pos());
		jgl::fill_rectangle(0, jgl::Vector2(g_application->size().x, start_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(0.0f, end_pos.y), jgl::Vector2(g_application->size().x, g_application->size().y - end_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(0.0f, start_pos.y), jgl::Vector2(start_pos.x, end_pos.y - start_pos.y), jgl::Color(0, 0, 0, 200));
		jgl::fill_rectangle(jgl::Vector2(end_pos.x, start_pos.y), jgl::Vector2(g_application->size().x - end_pos.x, end_pos.y - start_pos.y), jgl::Color(0, 0, 0, 200));
	}
	if (_turn_order != nullptr)
	{
		for (int i = 0; i < _turn_order->size(); i++)
		{
			_turn_order->operator[](i)->render(_viewport, _pointer->pos(), i == *_turn_index);
		}
	}
}