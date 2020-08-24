#ifndef ERELIA_ITEM_H
#define ERELIA_ITEM_H

#include "jgl.h"

#include "erelia_console.h"
#include "erelia_prefab.h"

enum class Item_type
{
	tile = 0,
	interact = 1,
	prefab = 2,
	npc = 3,
	count
};

class Item
{
protected:
	Item_type _item_type;
	jgl::Vector2 _sprite;
public:
	Item(Item_type p_item_type, jgl::Vector2 p_sprite);

	Item_type item_type() { return (_item_type); }
	jgl::Vector2 sprite() { return (_sprite); }

	virtual void draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, const jgl::Viewport* p_viewport);
	virtual void use(jgl::Data param) = 0;
};

class Node_item : public Item
{
private:
	size_t _node_index;

public:
	Node_item(size_t p_node_index, jgl::Vector2 p_sprite) : Item(Item_type::tile, p_sprite)
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
	Editor_item(jgl::Funct p_funct, jgl::Vector2 p_sprite) : Item(Item_type::interact, p_sprite)
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
	Prefab_item(Prefab* p_prefab) : Item(Item_type::prefab, jgl::Vector2(18, 71))
	{
		_prefab = p_prefab;
	}

	void use(jgl::Data param)
	{
		jgl::Vector2* start = param.acces<jgl::Vector2*>(0);
		_prefab->use(*start);
	}

	void draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, const jgl::Viewport* p_viewport);
};

class NPC_item : public Item
{
private:
	NPC* _npc;
public:
	NPC_item(NPC* p_prefab) : Item(Item_type::npc, jgl::Vector2(18, 71))
	{
		_npc = p_prefab;
	}

	void use(jgl::Data param);

	void draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, const jgl::Viewport* p_viewport);
};

#endif