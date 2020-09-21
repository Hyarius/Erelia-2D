#include "erelia.h"

jgl::Array<Creature*> creature_list;

void create_creature_list(jgl::Sprite_sheet* faceset)
{
	jgl::Array<jgl::File> file_list = jgl::list_files("ressources/creature/", ".crea");

	for (size_t i = 1; i < file_list.size(); i++)
	{
		jgl::String path = file_list[i].path + file_list[i].name + file_list[i].extension;
		creature_list.push_back(new Creature(path));
	}
}