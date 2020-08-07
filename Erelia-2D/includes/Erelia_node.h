#ifndef ERELIA_NODE_H
#define ERELIA_NODE_H

#include "erelia_tile.h"
#include "erelia_player.h"

class Link
{
private:
	jgl::Vector2 _A;
	jgl::Vector2 _B;
public:
	Link(jgl::Vector2 p_A, jgl::Vector2 p_B);
	jgl::Vector2 a() { return (_A); }
	jgl::Vector2 b() { return (_B); }
	void use(Player* player);
};

class Node
{
private:
	Tile* _tile;
	Link* _link;
	bool _occuped;
public:
	Node(Tile* p_tile)
	{
		_tile = p_tile;
		_link = nullptr;
		_occuped = false;
	}
	Tile* tile() { return (_tile); }
	Link* link() { return (_link); }
	bool occuped() { return (_occuped); }

	void set_tile(Tile* p_tile) { _tile = p_tile; }
	void set_link(Link* p_link) { _link = p_link; }
	void set_occuped(bool p_occuped) { _occuped = p_occuped; }
};

#endif