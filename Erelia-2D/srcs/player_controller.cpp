#include "erelia.h"

Player_controller::Player_controller(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{

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
			if (engine->player()->is_static() == true)
			{
				Node* tmp = engine->board()->node((engine->player()->pos() + move_delta[i]).round());
				Node* actual = engine->board()->node((engine->player()->pos()).round());
				if (engine->player()->ghost() == true)
					engine->player()->move(move_delta[i]);
				else if (engine->player()->can_move(move_delta[i]) == true)
				{
					jgl::Vector2 tmp_pos = move_delta[i];
					while ((engine->board()->node(engine->player()->pos().round() + tmp_pos)->tile()->type & JUMPING) == JUMPING)
						tmp_pos += move_delta[i];
					engine->board()->node(engine->player()->pos().round() + tmp_pos)->set_occupant(engine->player());
					engine->player()->move(tmp_pos);
				}
				else
					engine->player()->set_look_dir(look_dir_value[i]);
				return (true);
			}
			else
				return (false);
		}
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::release)
	{
		size_t i = 0;
		for (; i < 4 && look_dir_value[i] != engine->player()->look_dir(); i++);
		if (i != 4)
		{
			Node* tmp = engine->board()->node(engine->player()->pos() + move_delta[i]);
			if (tmp != nullptr && tmp->occupant() != nullptr && tmp->occupant() != engine->player() && tmp->occupant()->is_active() == false)
			{
				engine->interact_between(tmp->occupant(), engine->player());
				return (true);
			}
		}
	}
	return (false);
}

void Player_controller::update()
{
	engine->player()->update();
	Node* actual = engine->board()->node((engine->player()->pos()).round());
	if (engine->player()->is_static() == true && actual != nullptr && actual->link() != nullptr)
	{
		if (engine->player()->did_tp() == false)
		{
			engine->board()->node(engine->player()->pos().round())->set_occupant(nullptr);
			actual->link()->use(engine->board(), engine->player());
		}
	}

}

void Player_controller::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}

void Player_controller::render()
{

}