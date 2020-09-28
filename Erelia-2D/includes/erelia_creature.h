#ifndef ERELIA_CREATURE_H
#define ERELIA_CREATURE_H

#include "jgl.h"

struct Trait
{
	float actual;
	float upgrade_value;

	Trait(float p_value = 0, float p_upgrade_value = 0)
	{
		actual = p_value;
		upgrade_value = p_upgrade_value;
	}
	void upgrade(int nb_level = 1)
	{
		actual += upgrade_value * nb_level;
	}
};

struct Stat : public Trait
{
	float max;

	Stat(float p_value = 1.0f, float p_upgrade_value = 0.0f) : Trait(p_value, p_upgrade_value)
	{
		max = p_value;
	}
	Stat(float p_value, float p_max, float p_upgrade_value) : Trait(p_value, p_upgrade_value)
	{
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
	Stat _HP;
	Trait _attack;
	Trait _defense;
	Trait _attack_spe;
	Trait _defense_spe;
	Trait _initiative;

public:
	Creature(Creature* other);
	Creature(int p_id, jgl::String p_name, jgl::Vector2 p_face, jgl::Vector2 p_sprite);
	Creature(jgl::String path);

	const int id() const { return (_id); }
	const jgl::String name() const { return (_name); }
	const jgl::Vector2 face() const { return (_face); }
	const jgl::Vector2 sprite() const { return (_sprite); }

	const Stat PA() const { return (_PA); }
	const Stat HP() const { return (_HP); }
	const Stat PM() const { return (_PM); }
	const Trait attack() const { return (_attack); }
	const Trait defense() const { return (_defense); }
	const Trait attack_spe() const { return (_attack_spe); }
	const Trait defense_spe() const { return (_defense_spe); }
	const Trait initiative() const { return (_initiative); }
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

	int _level;

	Stat _PA;
	Stat _PM;
	Stat _HP;
	Trait _attack;
	Trait _defense;
	Trait _attack_spe;
	Trait _defense_spe;
	Trait _initiative;

public:
	Creature_entity(Creature* base, Team p_team);

	Stat& PA() { return (_PA); }
	Stat& PM() { return (_PM); }
	Stat& HP() { return (_HP); }
	Trait& attack() { return (_attack); }
	Trait& defense() { return (_defense); }
	Trait& attack_spe() { return (_attack_spe); }
	Trait& defense_spe() { return (_defense_spe); }
	Trait& initiative() { return (_initiative); }

	void reset_stat();
	void upgrade(int nb_level = 1);

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