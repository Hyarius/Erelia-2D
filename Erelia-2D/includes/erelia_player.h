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
	Player();
	bool ghost() { return (_ghost); }
	void set_ghost(bool p_state) { _ghost = p_state; }
	void update();
};

#endif