#ifndef ERELIA_NPC_H
#define ERELIA_NPC_H

#include "jgl.h"
#include "erelia_entity.h"

class NPC : public Entity
{
private:
	jgl::String _name;
	jgl::Vector2 _start_pos;
public:
	NPC(jgl::String p_name, jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite);

	jgl::String name() { return (_name); }
	void update(class Board* board);
	void save(std::fstream& file);
};

#endif