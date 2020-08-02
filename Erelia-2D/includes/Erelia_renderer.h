#ifndef ERELIA_RENDERER_H
#define ERELIA_RENDERER_H

#include "jgl.h"
#include "erelia_board.h"
#include "erelia_player.h"

class Renderer : public jgl::Widget
{
private:
	Board* _board;
	Player* _player;

public:
	Renderer(Board* p_board, Player* p_player, jgl::Widget* p_parent);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif