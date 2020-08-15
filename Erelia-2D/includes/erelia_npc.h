#ifndef ERELIA_NPC_H
#define ERELIA_NPC_H

#include "jgl.h"
#include "erelia_entity.h"

class NPC : public Entity
{
private:
	jgl::Vector2 _starting_pos;
	jgl::Array<jgl::Vector2> _check_point;
	size_t _check_point_index;
	jgl::Array<jgl::Vector2> _road;
	size_t _road_index;

public:
	NPC(jgl::String p_name, jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite);
	void add_check_point(jgl::Vector2 p_point) { _check_point.push_back(p_point); }
	void calc_next_road(class Board* board);
	void update(class Board* board);
	void save(std::fstream& file);

	jgl::Vector2 starting_pos() { return (_starting_pos); }


	void interact();
	jgl::Array<jgl::Vector2>& check_point() { return (_check_point); }
	jgl::Vector2 check_point(size_t i) { return (_check_point[i]); }
	jgl::Array<jgl::Vector2>& road() { return (_road); }
	jgl::Vector2 road(size_t i) { return (_road[i]); }
};

#endif