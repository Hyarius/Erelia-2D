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
	jgl::Vector2 _pos;
	Tile* _tile;
	Link* _link;
	bool _occuped;

	bool _calculated;
	size_t _s_cost;
	size_t _e_cost;
	size_t _t_cost;
	Node* _parent;

public:
	Node(Tile* p_tile, jgl::Vector2 p_pos)
	{
		_pos = p_pos;
		_tile = p_tile;
		_link = nullptr;
		_occuped = false;
		_calculated = false;
	}
	Tile* tile() { return (_tile); }
	Link* link() { return (_link); }
	bool occuped() { return (_occuped); }
	void set_calculated(bool p_state) { _calculated = p_state; }
	bool calculated() { return (_calculated); }
	Node* parent() { return (_parent); }
	void set_parent(Node* p_parent) { _parent = p_parent; }
	jgl::Vector2 pos() { return (_pos); }
	void reset_cost() { _s_cost = 0; _e_cost = 0; _t_cost = 0; _calculated = false; }
	void calc_cost(size_t start_cost, jgl::Vector2 end)
	{
		size_t e = static_cast<int>(std::abs(_pos.x - end.x)) + static_cast<int>(std::abs(_pos.y - end.y));

		_s_cost = start_cost;
		_e_cost = e;
		_t_cost = start_cost + e;
	}

	void set_s_cost(size_t p_cost) { _s_cost = p_cost; }
	void set_e_cost(size_t p_cost) { _e_cost = p_cost; }
	void set_t_cost(size_t p_cost) { _t_cost = p_cost; }

	size_t s_cost() { return (_s_cost); }
	size_t e_cost() { return (_e_cost); }
	size_t t_cost() { return (_t_cost); }

	void set_tile(Tile* p_tile) { _tile = p_tile; }
	void set_link(Link* p_link) { _link = p_link; }
	void set_occuped(bool p_occuped) { _occuped = p_occuped; }
};

#endif