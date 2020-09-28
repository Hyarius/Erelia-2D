#include "erelia.h"

Creature::Creature(Creature* other) : Creature(other->id(), other->name(), other->face(), other->sprite())
{
	_PA = other->PA();
	_PM = other->PM();
	_HP = other->HP();
	_attack = other->attack();
	_defense = other->defense();
	_attack_spe = other->attack_spe();
	_defense_spe = other->defense_spe();
	_initiative = other->initiative();
}

Creature::Creature(int p_id, jgl::String p_name, jgl::Vector2 p_face, jgl::Vector2 p_sprite)
{
	_id = p_id;
	_name = p_name;
	_face = p_face;
	_sprite = p_sprite;

	_PA = Stat(6, 0);
	_PM = Stat(3, 0);
	_HP = Stat(20, 0);
	_attack = Trait(10, 2);
	_defense = Trait(8, 2);
	_attack_spe = Trait(10, 2);
	_defense_spe = Trait(8, 2);
	_initiative = Trait(10, 2);
}

Creature::Creature(jgl::String path)
{
	std::fstream file = jgl::open_file(path, std::ios_base::in);

	jgl::String line = jgl::get_str(file);
	_id = jgl::stoi(line);

	_name = jgl::get_str(file);

	jgl::Array<jgl::String> tab = jgl::get_strsplit(file, ";", 2);
	_face = jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_sprite = jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_PA = Stat(jgl::stof(tab[0]), jgl::stof(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_PM = Stat(jgl::stof(tab[0]), jgl::stof(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_HP = Stat(jgl::stof(tab[0]), jgl::stof(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_attack = Trait(jgl::stof(tab[0]), jgl::stof(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_defense = Trait(jgl::stof(tab[0]), jgl::stof(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_attack_spe = Trait(jgl::stof(tab[0]), jgl::stof(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_defense_spe = Trait(jgl::stof(tab[0]), jgl::stof(tab[1]));

	tab = jgl::get_strsplit(file, ";", 2);
	_initiative = Trait(jgl::stof(tab[0]), jgl::stof(tab[1]));
}

std::ostream& operator<<(std::ostream& os, const Creature value)
{
	os << value.name() << "(" << value.id() << ")";
	return (os);
}

Creature_entity::Creature_entity(Creature* base, Team p_team) : Entity(Entity_type::Creature, base->name(), -1, base->sprite())
{
	set_move_speed(0.5f);
	set_wait_time(10);
	_species = base;
	_team = p_team;

	_level = 1;

	_PA = base->PA();
	_PM = base->PM();
	_HP = base->HP();
	_attack = base->attack();
	_defense = base->defense();
	_attack_spe = base->attack_spe();
	_defense_spe = base->defense_spe();
	_initiative = base->initiative();
}

void Creature_entity::reset_stat()
{
	_PA.reset();
	_PM.reset();
}

void Creature_entity::upgrade(int nb_level)
{
	_PA.upgrade(nb_level);
	_PM.upgrade(nb_level);
	_HP.upgrade(nb_level);
	_attack.upgrade(nb_level);
	_defense.upgrade(nb_level);
	_attack_spe.upgrade(nb_level);
	_defense_spe.upgrade(nb_level);
	_initiative.upgrade(nb_level);
}

void Creature_entity::render(jgl::Viewport* p_viewport, jgl::Vector2 base_pos, bool selected)
{
	Entity::render(p_viewport, base_pos);
	jgl::Vector2 tmp_pos = engine->board()->tile_to_screen(_pos, base_pos);
	if (selected == true)
		engine->battle_tileset()->draw(jgl::Vector2(static_cast<int>(Battle_node_type::select), 0), tmp_pos, node_size, 1.0f, p_viewport);
	else if (_team == Team::ally)
		engine->battle_tileset()->draw(jgl::Vector2(static_cast<int>(Battle_node_type::ally), 0), tmp_pos, node_size, 1.0f, p_viewport);
	else if (_team == Team::enemy)
		engine->battle_tileset()->draw(jgl::Vector2(static_cast<int>(Battle_node_type::enemy), 0), tmp_pos, node_size, 1.0f, p_viewport);
}