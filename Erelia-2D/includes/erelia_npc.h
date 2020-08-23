#ifndef ERELIA_NPC_H
#define ERELIA_NPC_H

#include "jgl.h"
#include "erelia_entity.h"

class NPC : public Entity
{
private:
	jgl::String _path;

public:
	NPC(jgl::String p_path, jgl::String p_name, jgl::Vector2 p_sprite);
	NPC(jgl::String p_path);
	jgl::String path(){ return (_path);}

	NPC* clone();
};

#endif