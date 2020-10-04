#ifndef ERELIA_PLAYER_H
#define ERELIA_PLAYER_H

#include "jgl.h"
#include "erelia_define.h"
#include "erelia_entity.h"

struct Team_comp
{
	size_t size;
	class Creature_entity** unit;

	Team_comp(size_t p_size = 6)
	{
		size = p_size;
		unit = new class Creature_entity* [size];
		for (size_t i = 0; i < size; i++)
			unit[i] = nullptr;
	}
	void define(size_t index, class Creature_entity* creature)
	{
		if (index >= size)
			return;
		unit[index] = creature;
	}
};

class Player : public Entity
{
private:
	bool _ghost;
	Team_comp* _team;

public:
	Player();
	bool ghost() { return (_ghost); }
	Team_comp* team() { return (_team); }
	void set_ghost(bool p_state) { _ghost = p_state; }
	void update();
};

#endif