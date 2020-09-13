#include "erelia.h"

void define_first_coord(jgl::Data param)
{

	engine->editor_mode()->editor_interacter()->pink_flag() = screen_to_tile(g_mouse->pos);
}

void define_second_coord(jgl::Data param)
{
	engine->editor_mode()->editor_interacter()->blue_flag() = screen_to_tile(g_mouse->pos);
}

void define_selected_npc(jgl::Data param)
{
	jgl::Vector2 pos = screen_to_tile(g_mouse->pos);
	Entity* tmp = engine->board()->node(pos)->occupant();
	engine->editor_mode()->editor_interacter()->selected_entity() = tmp;
}

void add_selected_npc_check_point(jgl::Data param)
{
	Entity* select_npc = engine->editor_mode()->editor_interacter()->selected_entity();

	if (select_npc == nullptr)
		return;
	jgl::Vector2 pos = screen_to_tile(g_mouse->pos);
	(static_cast<NPC*>(select_npc))->add_check_point(pos);
}

jgl::Funct funct_array[18] = {
	define_first_coord,
	define_second_coord,
	define_selected_npc,
	add_selected_npc_check_point,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr
};