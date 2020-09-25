#ifndef ERELIA_NODE_H
#define ERELIA_NODE_H

#include "erelia_tile.h"
#include "erelia_player.h"

class Base_node
{
protected:
	jgl::Vector2 _pos;
	class Entity* _occupant;

	bool _calculated;
	int _distance;
	size_t _s_cost;
	size_t _e_cost;
	size_t _t_cost;
	Base_node* _parent;

public:
	Base_node(jgl::Vector2 p_pos)
	{
		_pos = p_pos;
		_calculated = false;
		_distance = 0;
		_s_cost = 0;
		_e_cost = 0;
		_t_cost = 0;
		_parent = nullptr;
		_occupant = nullptr;
	}

	class Entity* occupant() { return (_occupant); }
	bool occuped() { return (_occupant != nullptr); }
	void set_calculated(bool p_state) { _calculated = p_state; }
	bool calculated() { return (_calculated); }
	Base_node* parent() { return (_parent); }
	void set_parent(Base_node* p_parent) { _parent = p_parent; }
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

	void set_occupant(class Entity* p_occupant) { _occupant = p_occupant; }
};

class Link
{
private:
	jgl::Vector2 _A;
	jgl::Vector2 _B;
public:
	Link(jgl::Vector2 p_A, jgl::Vector2 p_B);
	jgl::Vector2 a() { return (_A); }
	jgl::Vector2 b() { return (_B); }
	void use(class Board* board, Player* player);
};

class Node : public Base_node
{
private:
	Tile* _tile;
	Link* _link;
	class Battle_data* _encounter_area;

public:
	Node(Tile* p_tile, jgl::Vector2 p_pos) : Base_node(p_pos)
	{
		_tile = p_tile;
		_link = nullptr;
		_encounter_area = nullptr;
	}
	Tile* tile() { return (_tile); }
	Link* link() { return (_link); }
	class Battle_data* encounter_area() { return (_encounter_area); }

	void set_tile(Tile* p_tile) { _tile = p_tile; }
	void set_link(Link* p_link) { _link = p_link; }
	void set_encounter_area(class Battle_data* p_encounter_area) { _encounter_area = p_encounter_area; }
};

#endif