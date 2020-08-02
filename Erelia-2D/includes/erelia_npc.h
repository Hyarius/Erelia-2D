#ifndef ERELIA_NPC_H
#define ERELIA_NPC_H

#include "jgl.h"
#include "erelia_entity.h"

class NPC : public Entity
{
private:
	jgl::String _name;
	jgl::Array<jgl::String> _dialogue;
public:
	NPC(jgl::String p_name, jgl::Array<jgl::String> p_dialogue, jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite);
	void interact();

	void save(std::fstream& file);
};

#endif