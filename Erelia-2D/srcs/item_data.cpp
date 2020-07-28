#include "erelia.h"

jgl::Array<jgl::String> item_type_name = {
	"Node", "Env.", "Trainer", "Interact"
};

jgl::Array<Item*> node_item_list;
jgl::Array<Item*> env_item_list;
jgl::Array<Item*> trainer_item_list;
jgl::Array<Item*> interact_item_list;

jgl::Array<jgl::Array<Item*> *> item_array = {
	& node_item_list, &env_item_list, &trainer_item_list, &interact_item_list
};

void create_node_item_list(jgl::Sprite_sheet* tileset)
{
	for (size_t i = 0; i < node_array.size(); i++)
	{
		node_item_list.push_back(new Node_item(i, tileset, node_array[i]->sprite));
	}
}