#include "erelia.h"

Player_controller::Player_controller(Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_board = p_board;
	_player = p_player;
}

bool Player_controller::handle_keyboard()
{
	static jgl::key key_value[4] = { jgl::key::s, jgl::key::w, jgl::key::a, jgl::key::d };
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };
	static node_type type[4] = { DOWN_WALKABLE, UP_WALKABLE, LEFT_WALKABLE, RIGHT_WALKABLE };
	static node_type rev_type[4] = { UP_WALKABLE, DOWN_WALKABLE, RIGHT_WALKABLE, LEFT_WALKABLE };
	for (size_t i = 0; i < 4; i++)
	{
		if (jgl::get_key(key_value[i]) == jgl::key_state::down)
		{
			if (_player->ghost() == true)
			{
				_player->move(move_delta[i]);
			}
			else
			{
				Node* actual = _board->node((_player->pos()).round());
				Node* tmp = _board->node((_player->pos() + move_delta[i]).round());
				if (tmp != nullptr)
				{
					if ((tmp->type & rev_type[i]) == rev_type[i] && (actual->type & type[i]) == type[i])
						_player->move(move_delta[i]);
				}
			}
			return (true);
		}
	}
	return (false);
}

void Player_controller::update()
{
	_player->update();
}

void Player_controller::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}

void Player_controller::render()
{

}