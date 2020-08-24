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

	tab = jgl::get_strsplit(tmp_file, ";", 2);
	size_t nb_interaction = jgl::stoi(tab[1]);
	for (size_t i = 0; i < nb_interaction; i++)
	{
		tab = jgl::get_strsplit(tmp_file, ";");
		int index = jgl::stoi(tab[0]);
		int result_type = jgl::stoi(tab[1]);
		if (result_type < 0 || result_type >= static_cast<int>(Interaction_type::count))
			jgl::error_exit(1, "Bad interaction type found");
		Interaction_type type = static_cast<Interaction_type>(result_type);
		Interaction result = Interaction(type);

		I_funct tmp = interact_parse_tab[result_type];
		((&result)->*tmp)(tab, 2);

		add_interaction(index, result);
	}

	set_move_speed(0.4f);
	set_wait_time(450);
}


NPC* NPC::clone()
{
	NPC* result = new NPC(_path, _name, _sprite);
	for (auto tmp : _interaction)
		result->add_interaction(tmp.first, tmp.second);

	return (result);
}