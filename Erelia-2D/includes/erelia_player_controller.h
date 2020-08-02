#ifndef ERELIA_PLAYER_CONTROLLER_H
#define ERELIA_PLAYER_CONTROLLER_H

#include "jgl.h"
#include "erelia_player.h"

class Player_controller : public jgl::Widget
{
private:
	Board* _board;
	Player* _player;

public:
	Player_controller(Board* p_board, Player* p_player, jgl::Widget* p_parent = nullptr);
	bool handle_keyboard();
	void update();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif