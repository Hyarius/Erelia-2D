#ifndef ERELIA_ADVENTURE_MENU_H
#define ERELIA_ADVENTURE_MENU_H

#include "jgl.h"

class Creature_slot : public jgl::Widget
{
private:
	bool _selected;
	jgl::w_box_component _box;
	class Creature_entity** _creature;

public:
	Creature_slot(class Creature_entity** p_creature, jgl::Widget* p_parent = nullptr);
	class Creature_entity** creature() { return (_creature); }
	void select();
	void unselect();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Adventure_menu : public jgl::Widget
{
private:
	Creature_slot* _selected_slot;
	jgl::Frame* _frame;
	Creature_slot* _slots[6];

public:
	Adventure_menu(jgl::Widget* p_parent = nullptr);
	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif