#include "erelia.h"

Player_controller::Player_controller(Board* p_board, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_board = p_board;
	_player = p_player;
}

bool Player_controller::handle_keyboard()
{
	static jgl::key key_value[4] = { jgl::key::s, jgl::key::w, jgl::key::a, jgl::key::d };
	static Entity_direction look_dir_value[4] = { Entity_direction::south, Entity_direction::north, Entity_direction::west, Entity_direction::east};
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };
	
	for (size_t i = 0; i < 4; i++)
	{
		if (jgl::get_key(key_value[i]) == jgl::key_state::down)
		{
			if (_player->is_static() == true)
			{
				Node* tmp = _board->node((_player->pos() + move_delta[i]).round());
				Node* actual = _board->node((_player->pos()).round());
				if (_player->ghost() == true)
					_player->move(_board, move_delta[i]);
				else if (_player->can_move(_board, move_delta[i]) == true)
				{
					jgl::Vector2 tmp_pos = move_delta[i];
					while ((_board->node(_player->pos().round() + tmp_pos)->tile()->type & JUMPING) == JUMPING)
						tmp_pos += move_delta[i];
					_board->node(_player->pos().round() + tmp_pos)->set_occupant(_player);
					_player->move(_board, tmp_pos);
				}
				else
					_player->set_look_dir(look_dir_value[i]);
				return (true);
			}
			else
				return (false);
		}
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::down)
	{
		size_t i = 0;
		for (; i < 4 && look_dir_value[i] != _player->look_dir(); i++);
		if (i != 4)
		{
			Node* tmp = _board->node(_player->pos() + move_delta[i]);
			if (tmp != nullptr && tmp->occupant() != nullptr && tmp->occupant() != _player)
				tmp->occupant()->interact();
		}
	}
	return (false);
}

void Player_controller::update()
{
	Node* actual = _board->node((_player->pos()).round());
	if (_player->is_static() == true && actual != nullptr && actual->link() != nullptr)
	{
		if (_player->did_tp() == false)
		{
			_board->node(_player->pos().round())->set_occupant(nullptr);
			actual->link()->use(_board, _player);
		}
	}

}

void Player_controller::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}

void Player_controller::render()
{

}