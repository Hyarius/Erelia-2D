#ifndef ERELIA_CREATURE_H
#define ERELIA_CREATURE_H

#include "jgl.h"

enum class Creature_family
{
	Vegetaux,
	Dragon,
	Bete,
	Insecte,
	Aerien,
	Reptile,
	Mystique,
	Humanoide,
	Elementaire,
	Objet,
	Fossile,
	Parasite,
	Legendaire
};

enum class Creature_type
{

};

using Attack_type = Creature_type;

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