#include "erelia.h"

NPC::NPC(jgl::String p_name, jgl::Vector2 p_pos, jgl::Vector2 p_sprite) : Entity(Entity_type::NPC, p_name, p_pos, p_sprite)
{
	set_move_speed(0.4f);
	set_wait_time(450);
	_movement_type = Entity_movement::errant;
}

void NPC::save(std::fstream& file)
{
	Entity::save(file);
}

void NPC::load(std::fstream& file)
{
	jgl::Array<jgl::String> tab = jgl::get_strsplit(file, ";");
	size_t i = Entity::load_from_line(tab);
	if (i < tab.size())
	{
		size_t nb_interact = jgl::stoi(tab[i]);
		i++;
		for (size_t j = 0; j < nb_interact; j++)
		{
			Interaction_type type = static_cast<Interaction_type>(jgl::stoi(tab[i]));
			Interaction inter = Interaction(type);
			I_funct funct = interact_parse_tab[static_cast<size_t>(type)];
			i++;
			i += (inter.*funct)(tab, i);
			_interaction.push_back(inter);
		}
	}
}