#include "erelia.h"

NPC::NPC(jgl::String p_path, jgl::String p_name, jgl::Vector2 p_sprite) : Entity(Entity_type::NPC, p_name, -1, p_sprite)
{
	_path = p_path;
	set_move_speed(0.4f);
	set_wait_time(450);
}

NPC::NPC(jgl::String p_path) : Entity(Entity_type::NPC, "Unnamed", -1, 0)
{
	_path = p_path;
	std::fstream tmp_file = jgl::open_file(p_path, std::ios_base::in);

	_name = jgl::get_str(tmp_file);
	jgl::Array<jgl::String> tab = jgl::get_strsplit(tmp_file, ";", 2);
	_sprite = jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1]));
	set_move_speed(0.4f);
	set_wait_time(450);
}


NPC* NPC::clone()
{
	NPC* result = new NPC(_path, _name, _sprite);
	for (size_t i = 0; i < _interaction.size(); i++)
		result->add_interaction(_interaction[i]);

	return (result);
}