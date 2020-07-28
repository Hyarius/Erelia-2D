#ifndef ERELIA_PLAYER_CONTROLLER_H
#define ERELIA_PLAYER_CONTROLLER_H

#include "jgl.h"
#include "erelia_player.h"

class Player_controller : public jgl::Widget
{
private:
	Player* _player;
public:
	Player_controller(Player* p_player, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_player = p_player;
	}

	bool handle_keyboard()
	{
		static jgl::key key_value[4] = { jgl::key::s, jgl::key::w, jgl::key::a, jgl::key::d };
		static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 0.5f), jgl::Vector2(0.0f, -0.5f), jgl::Vector2(-0.5f, 0.0f), jgl::Vector2(0.5f, 0.0f) };

		for (size_t i = 0; i < 4; i++)
		{
			if (jgl::get_key(key_value[i]) == jgl::key_state::down)
			{
				_player->move(move_delta[i]);
				return (true);
			}
		}
		return (false);
	}

	void update()
	{
		_player->update();
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{

	}

	void render()
	{

	}
};

#endif