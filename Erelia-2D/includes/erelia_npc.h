#ifndef ERELIA_NPC_H
#define ERELIA_NPC_H

#include "jgl.h"
#include "erelia_entity.h"

class NPC : public Entity
{
private:
public:
	NPC(jgl::String p_name = "", jgl::Vector2 p_pos = 0, jgl::Vector2 p_sprite = 0);

	void save(std::fstream& file);
	void load(std::fstream& file);
};

#endif