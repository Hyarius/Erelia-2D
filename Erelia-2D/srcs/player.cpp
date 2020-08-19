#include "erelia.h"

Player::Player() : Entity(Entity_type::Player, "Player", 0, 0)
{
	_ghost = false;
	set_move_tick(150);
}

void Player::update()
{
	if (is_moving() == true)
	{
		update_pos();
	}
}