#ifndef ERELIA_ADVENTURE_MENU_H
#define ERELIA_ADVENTURE_MENU_H

#include "jgl.h"

class Creature_slot : public jgl::Widget
{
private:
	bool _selected;
	jgl::w_box_component _box;
	class Creature_entity** _creature;
	jgl::Progressive_bar* _hp_bar;

public:
	Creature_slot(class Creature_entity** p_creature, jgl::Widget* p_parent = nullptr);
	class Creature_entity** creature() { return (_creature); }
	void update();
	void select();
	void unselect();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Creature_battle_slot : public jgl::Widget
{
private:
	bool _selected;
	jgl::w_box_component _box;
	class Creature_entity** _creature;
	jgl::Progressive_bar* _hp_bar;

public:
	Creature_battle_slot(class Creature_entity** p_creature, jgl::Widget* p_parent = nullptr);
	class Creature_entity** creature() { return (_creature); }
	void update();
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

class Battle_change_menu : public jgl::Widget
{
private:
	Creature_battle_slot* _selected_slot;
	Creature_battle_slot* _slots[6];
	jgl::Frame* _info_frame;
	jgl::Text_label* _creature_name;
	jgl::Text_label* _creature_PA;
	jgl::Text_label* _creature_PM;
	jgl::Text_label* _creature_HP;

public:
	Battle_change_menu(jgl::Widget* p_parent = nullptr);
	Creature_battle_slot* selected_slot() { return (_selected_slot); }
	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif