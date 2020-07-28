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
	Renderer(Board* p_board, Player* p_player, jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_board = p_board;
		_player = p_player;
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{

	}

	void render()
	{
		_board->render(_player->pos(), _viewport);

		jgl::fill_centred_rectangle(g_application->size() / 2, 2, jgl::Color(255, 0, 0), _viewport);
		jgl::draw_text("Fps : " + jgl::itoa(print_fps), 50, 16, 1, jgl::text_color::white, jgl::text_style::normal, _viewport);
	}
};

#endif