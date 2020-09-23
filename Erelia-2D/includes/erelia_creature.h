#ifndef ERELIA_CREATURE_H
#define ERELIA_CREATURE_H

#include "jgl.h"

struct Stat
{
	int actual;
	int max;

	Stat(int value = -1)
	{
		actual = value;
		max = value;
	}
	Stat(int p_actual, int p_max)
	{
		actual = p_actual;
		max = p_max;
	}
	void reset()
	{
		actual = max;
	}
};

class Creature
{
private:
	int _id;
	jgl::String _name;
	jgl::Vector2 _face;
	jgl::Vector2 _sprite;

	Stat _PA;
	Stat _PM;

public:
	Creature(Creature* other);
	Creature(int p_id, jgl::String p_name, jgl::Vector2 p_face, jgl::Vector2 p_sprite);
	Creature(jgl::String path);

	const int id() const { return (_id); }
	const jgl::String name() const { return (_name); }
	const jgl::Vector2 face() const { return (_face); }
	const jgl::Vector2 sprite() const { return (_sprite); }

	const Stat PA() const { return (_PA); }
	const Stat PM() const { return (_PM); }
};

extern jgl::Array<Creature*> creature_list;

enum class Team
{
	ally = 0,
	enemy = 1,
	neutral = 2
};

class Creature_entity : public Entity
{
private:
	Creature* _species;
	Team _team;

	Stat _PA;
	Stat _PM;

public:
	Creature_entity(Creature* base, Team p_team) : Entity(Entity_type::Creature, base->name(), -1, base->sprite())
	{
		set_move_speed(0.5f);
		_species = base;
		_team = p_team;
		_PA = base->PA();
		_PM = base->PM();
	}

	Stat& PA() { return (_PA); }
	Stat& PM() { return (_PM); }

	void reset_stat();

	void render(jgl::Viewport* p_viewport, jgl::Vector2 base_pos, bool selected);
	Creature* species() { return (_species); }
	Team team() { return (_team); }
};

void create_creature_list(jgl::Sprite_sheet* faceset);

struct Team_comp
{
	jgl::Array<Creature_entity*> unit;

	Team_comp(jgl::Array<Creature_entity*> element)
	{
		unit.resize(6);
		for (size_t i = 0; i < 6 && i < element.size(); i++)
			unit[i] = element[i];
	}
	void define(size_t index, Creature_entity* creature)
	{
		if (index >= 6)
			return;
		unit[index] = creature;
	}
};

std::ostream& operator<<(std::ostream& os, const Creature value);

#endif