#include "erelia.h"

Creature::Creature(Creature* other)
{
	_id = other->id();
	_name = other->name();
	_face = other->face();
	_sprite = other->sprite();
}

Creature::Creature(int p_id, jgl::String p_name, jgl::Vector2 p_face, jgl::Vector2 p_sprite)
{
	_id = p_id;
	_name = p_name;
	_face = p_face;
	_sprite = p_sprite;
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

	_PA = Stat(jgl::stoi(jgl::get_str(file)));
	_PM = Stat(jgl::stoi(jgl::get_str(file)));
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
	_PA = base->PA();
	_PM = base->PM();
}

void Creature_entity::reset_stat()
{
	_PA.reset();
	_PM.reset();
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