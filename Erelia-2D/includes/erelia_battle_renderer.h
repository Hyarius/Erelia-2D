#ifndef ERELIA_BATTLE_RENDERER_H
#define ERELIA_BATTLE_RENDERER_H

#include "erelia_battler.h"

class Battle_arena_renderer : public jgl::Widget
{
private:
	Battle_arena* _arena;
	Entity* _pointer;
	jgl::Array<Creature_entity*>* _turn_order;
	int* _turn_index;

public:
	Battle_arena_renderer(jgl::Widget* parent = nullptr);
	void set_arena(Battle_arena* p_arena);
	void set_turn_order(jgl::Array<Creature_entity*>* p_array);
	void set_pointer(Entity* p_pointer);
	void set_turn_index(int* p_turn_index);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Battle_face_renderer : public jgl::Widget
{
private:
	jgl::Frame* _frame;
	Creature_entity** _ally_entity;
	Creature_entity** _enemy_entity;
	Creature_entity* _old_ally_entity;
	Creature_entity* _old_enemy_entity;

	jgl::Text_label* _ally_name;
	jgl::Image_label* _ally_face;
	jgl::Progressive_bar* _ally_hp_bar;

	jgl::Text_label* _enemy_name;
	jgl::Image_label* _enemy_face;
	jgl::Progressive_bar* _enemy_hp_bar;

public:
	Battle_face_renderer(Creature_entity** p_ally_entity, Creature_entity** p_enemy_entity, jgl::Widget* parent = nullptr);
	void update();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif