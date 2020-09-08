#ifndef ERELIA_CREATURE_H
#define ERELIA_CREATURE_H

#include "jgl.h"

class Creature
{
private:
	int _id;
	jgl::String _name;

public:
	Creature(int p_id, jgl::String p_name)
	{
		_id = p_id;
		_name = p_name;
	}
	int id() { return (_id); }
	jgl::String name() { return (_name); }
};

void create_creature_list(jgl::Sprite_sheet* faceset);

#endif