#ifndef ERELIA_NODE_H
#define ERELIA_NODE_H

#include "jgl.h"
#include "erelia_define.h"

struct Node
{
	int index;
	jgl::Vector2 sprite;
	node_type type;

	Node(int p_index, jgl::Vector2 p_sprite, node_type p_type)
	{
		index = p_index;
		sprite = p_sprite;
		type = p_type;
	}
};

extern jgl::Array<Node*> node_array;
#endif