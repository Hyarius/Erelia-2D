#ifndef ERELIA_BOARD_RENDERER_H
#define ERELIA_BOARD_RENDERER_H

#include "jgl.h"

class Board_renderer : public jgl::Widget
{
private:
public:
	Board_renderer(jgl::Widget* p_parent = nullptr);
	void rebake();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif