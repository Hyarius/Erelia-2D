#include "erelia.h"

jgl::Array<jgl::String> item_type_name = {
	"Tile", "Env.", "Trainer", "Interact", "Prefab", "NPC"
};

jgl::Array<Item*> node_item_list;
jgl::Array<Item*> env_item_list;
jgl::Array<Item*> trainer_item_list;
jgl::Array<Item*> interact_item_list;
jgl::Array<Item*> prefab_item_list;
jgl::Array<Item*> npc_item_list;

jgl::Array<jgl::Array<Item*> *> item_array = {
	&node_item_list, &env_item_list, &trainer_item_list, &interact_item_list, &prefab_item_list, &npc_item_list
};


extern jgl::Funct funct_array[19];

static void create_prefab_list()
{
	jgl::Array<jgl::File> list = jgl::list_files("ressources/prefab", ".prefab");
	for (size_t i = 1; i < list.size(); i++)
	{
		jgl::String path = list[i].path + "/" + list[i].name + list[i].extension;
		Prefab* new_prefab = new Prefab(path);
		prefab_array.push_back(new_prefab);
		Prefab_item* new_item = new Prefab_item(new_prefab);
		prefab_item_list.push_back(new_item);
	}
}

static void create_npc_list()
{
	jgl::Array<jgl::File> list = jgl::list_files("ressources/npc", ".npc");
	for (size_t i = 1; i < list.size(); i++)
	{
		jgl::String path = list[i].path + "/" + list[i].name + list[i].extension;
		NPC* new_npc = new NPC(path);
		NPC_array.push_back(new_npc);
		NPC_item* new_item = new NPC_item(new_npc);
		npc_item_list.push_back(new_item);
	}
}

void create_item_list(jgl::Sprite_sheet* tileset)
{
	for (size_t i = 0; i < tile_array.size(); i++)
	{
		node_item_list.push_back( new Node_item(i, tile_array[i]->sprite) );
	}
	for (int i = 0; i < 18; i++)
	{
		jgl::Vector2 sprite = jgl::Vector2(i, 71);
		interact_item_list.push_back( new Editor_item(funct_array[i], sprite) );
	}
	create_prefab_list();
	create_npc_list();
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