#include "erelia.h"

void define_first_coord(jgl::Data param)
{
	Board* board = param.acces<Board*>(0);
	Player* player = param.acces<Player*>(1);
	jgl::Vector2* first = param.acces<jgl::Vector2*>(2);
	jgl::Vector2* second = param.acces<jgl::Vector2*>(3);

	*first = board->tile_coord(player->pos(), g_mouse->pos);
}

void define_second_coord(jgl::Data param)
{
	Board* board = param.acces<Board*>(0);
	Player* player = param.acces<Player*>(1);
	jgl::Vector2* first = param.acces<jgl::Vector2*>(2);
	jgl::Vector2* second = param.acces<jgl::Vector2*>(3);

	*second = board->tile_coord(player->pos(), g_mouse->pos);
}

jgl::Funct funct_array[18] = {
	define_first_coord,
	define_second_coord,
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
	nullptr,
	nullptr,
	nullptr
};