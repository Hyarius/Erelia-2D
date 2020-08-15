#include "erelia.h"

Player::Player(jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite) : Entity(Entity_type::Player, "Player", 0, p_charset, p_sprite)
{
	_ghost = false;
}

void Player::update(Board* board)
{
	if (is_moving() == true)
	{
		update_pos(board);
	}
}