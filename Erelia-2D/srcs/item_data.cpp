#include "erelia.h"

jgl::Array<jgl::String> item_type_name = {
	"Node", "Env.", "Trainer", "Interact", "Prefab"
};

jgl::Array<Item*> node_item_list;
jgl::Array<Item*> env_item_list;
jgl::Array<Item*> trainer_item_list;
jgl::Array<Item*> interact_item_list;
jgl::Array<Item*> prefab_item_list;
jgl::Array<Item*> npc_item_list;

jgl::Array<jgl::Array<Item*> *> item_array = {
	& node_item_list, &env_item_list, &trainer_item_list, &interact_item_list, &prefab_item_list, &npc_item_list
};

extern jgl::Funct funct_array[19];

void create_item_list(jgl::Sprite_sheet* tileset)
{
	for (size_t i = 0; i < node_array.size(); i++)
	{
		node_item_list.push_back( new Node_item(i, tileset, node_array[i]->sprite) );
	}
	for (size_t i = 0; i < 18; i++)
	{
		jgl::Vector2 sprite = jgl::Vector2(static_cast<float>(i), tileset->size().y - 1);
		interact_item_list.push_back( new Editor_item(funct_array[i], tileset, sprite) );
	}
	jgl::Array<jgl::File> list = jgl::list_files("ressources/prefab", ".prefab");
	for (size_t i = 1; i < list.size(); i++)
	{
		jgl::String path = list[i].path + "/" + list[i].name + list[i].extension;
		Prefab* new_prefab = new Prefab(path);
		prefab_array.push_back(new_prefab);
		Prefab_item* new_item = new Prefab_item(new_prefab, tileset);
		prefab_item_list.push_back(new_item);
	}
}

void delete_item_list()
{
	for (size_t t = 0; t < item_array.size(); t++)
	{
		for (size_t i = 0; i < item_array[t]->size(); i++)
		{
			delete item_array[t]->operator[](i);
		}
	}
}