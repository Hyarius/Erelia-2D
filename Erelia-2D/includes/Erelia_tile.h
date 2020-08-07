#ifndef ERELIA_TILE_H
#define ERELIA_TILE_H

#include "jgl.h"
#include "erelia_define.h"

struct Tile
{
	int index;
	jgl::Vector2 sprite;
	node_type type;

	Tile(int p_index, jgl::Vector2 p_sprite, node_type p_type)
	{
		index = p_index;
		sprite = p_sprite;
		type = p_type;
	}
};

extern jgl::Array<Tile*> tile_array;

#endif