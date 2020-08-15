#ifndef ERELIA_INTERACTER_H
#define ERELIA_INTERACTER_H

#include "jgl.h"

class Interacter : public jgl::Widget
{
private:
	Entity* _entity;
	jgl::Frame* _dialogue_frame;
public:
	Interacter(jgl::Widget* p_parent = nullptr);

	void set_entity(Entity* p_entity) { _entity = p_entity; }

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif