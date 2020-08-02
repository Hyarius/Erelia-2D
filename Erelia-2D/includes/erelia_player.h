#ifndef ERELIA_PLAYER_H
#define ERELIA_PLAYER_H

#include "jgl.h"
#include "erelia_define.h"
#include "erelia_entity.h"

class Player : public Entity
{
private:
	bool _ghost;
public:
	Player(jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite);
	bool ghost() { return (_ghost); }
	void set_ghost(bool p_state) { _ghost = p_state; }
	void interact();
};

#endif