#ifndef ERELIA_ITEM_H
#define ERELIA_ITEM_H

#include "jgl.h"

#include "erelia_console.h"
#include "erelia_prefab.h"

enum class Item_type
{
	node = 0,
	interact = 1,
	prefab = 2,
	count
};

class Item
{
protected:
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

	virtual void draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, const jgl::Viewport* p_viewport)
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

class Editor_item : public Item
{
private:
	jgl::Funct _funct;

public:
	Editor_item(jgl::Funct p_funct, jgl::Sprite_sheet* p_tileset, jgl::Vector2 p_sprite) : Item(Item_type::interact, p_tileset, p_sprite)
	{
		_funct = p_funct;
	}

	void use(jgl::Data param)
	{
		if (_funct != nullptr)
			_funct(param);
	}
};

class Prefab_item : public Item
{
private:
	Prefab* _prefab;

public:
	Prefab_item(Prefab* p_prefab, jgl::Sprite_sheet* p_tileset) : Item(Item_type::prefab, p_tileset, jgl::Vector2(18, 59))
	{
		_prefab = p_prefab;
	}

	void use(jgl::Data param)
	{
		Board* board = param.acces<Board*>(0);
		jgl::Vector2* start = param.acces<jgl::Vector2*>(1);
		_prefab->use(board, *start);
	}

	void draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, const jgl::Viewport* p_viewport)
	{
		_tileset->draw(_sprite, p_anchor, p_area, 1.0f, p_viewport);
		jgl::draw_centred_text(_prefab->name(), p_anchor + p_area / 2, 16, 1, jgl::text_color::white, jgl::text_style::normal, nullptr);
	}
};

#endif