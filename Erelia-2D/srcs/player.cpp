#include "erelia.h"

Player::Player() : Entity(Entity_type::Player, "Player", 0, 0)
{
	_ghost = false;
	_team = new Team_comp();
	for (size_t i = 0; i < 6; i++)
	_team->define(i, nullptr);
	set_move_tick(150);
}

void Player::update()
{
	if (is_moving() == true)
	{
		update_pos();
	}
}