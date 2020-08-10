#include "erelia.h"

void define_first_coord(jgl::Data param)
{
	Board* board = param.acces<Board*>(0);
	Player* player = param.acces<Player*>(1);
	jgl::Vector2* first = param.acces<jgl::Vector2*>(2);
	jgl::Vector2* second = param.acces<jgl::Vector2*>(3);

	*first = screen_to_tile(player->pos(), g_mouse->pos);
}

void define_second_coord(jgl::Data param)
{
	Board* board = param.acces<Board*>(0);
	Player* player = param.acces<Player*>(1);
	jgl::Vector2* first = param.acces<jgl::Vector2*>(2);
	jgl::Vector2* second = param.acces<jgl::Vector2*>(3);

	*second = screen_to_tile(player->pos(), g_mouse->pos);
}

void define_selected_npc(jgl::Data param)
{
	Board* board = param.acces<Board*>(0);
	Player* player = param.acces<Player*>(1);
	jgl::Vector2* first = param.acces<jgl::Vector2*>(2);
	jgl::Vector2* second = param.acces<jgl::Vector2*>(3);
	Entity** select_npc = param.acces<Entity**>(4);

	jgl::Vector2 pos = screen_to_tile(player->pos(), g_mouse->pos);
	Entity* tmp = board->node(pos)->occupant();
	*select_npc = tmp;
}

void add_selected_npc_check_point(jgl::Data param)
{
	Board* board = param.acces<Board*>(0);
	Player* player = param.acces<Player*>(1);
	jgl::Vector2* first = param.acces<jgl::Vector2*>(2);
	jgl::Vector2* second = param.acces<jgl::Vector2*>(3);
	Entity** select_npc = param.acces<Entity**>(4);

	if (*select_npc == nullptr)
		return;
	jgl::Vector2 pos = screen_to_tile(player->pos(), g_mouse->pos);
	(static_cast<NPC*>(*select_npc))->add_check_point(pos);
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