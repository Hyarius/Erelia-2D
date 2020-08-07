#include "erelia.h"

Link::Link(jgl::Vector2 p_A, jgl::Vector2 p_B)
{
	_A = p_A;
	_B = p_B;
}

void Link::use(Player* player)
{
	if (player->pos().round() == _A.round())
		player->place(_B);
	else
		player->place(_A);
}