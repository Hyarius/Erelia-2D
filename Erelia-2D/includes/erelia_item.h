#ifndef ERELIA_ITEM_H
#define ERELIA_ITEM_H

#include "jgl.h"

enum class Item_type
{
	node = 0,
	count
};

class Item
{
private:
	Item_type _item_type;
	jgl::Sprite_sheet* _tileset;
	jgl::Vector2 _sprite;
public:
	Item(Item_type p_item_type, jgl::Sprite_sheet* p_tileset, jgl::Vector2 p_sprite)
	{
		_tileset = p_tileset;
		_item_type = p_item_type;
		_sprite = p_sprite;
	}

	Item_type item_type() { return (_item_type); }
	jgl::Sprite_sheet* tileset() { return (_tileset); }
	jgl::Vector2 sprite() { return (_sprite); }

	void draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, jgl::Viewport* p_viewport)
	{
		_tileset->draw(_sprite, p_anchor, p_area, 1.0f, p_viewport);
	}
	virtual void use(jgl::Data param) = 0;
};

class Node_item : public Item
{
private:
	size_t _node_index;

public:
	Node_item(size_t p_node_index, jgl::Sprite_sheet* p_tileset, jgl::Vector2 p_sprite) : Item(Item_type::node, p_tileset, p_sprite)
	{
		_node_index = p_node_index;
	}

	size_t node_index() { return (_node_index); }

	void use(jgl::Data param)
	{
		Board* board = param.acces<Board*>(0);
		jgl::Vector2* pos = param.acces<jgl::Vector2*>(1);
		board->place(*pos, _node_index);
	}
};

#endif