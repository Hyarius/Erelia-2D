#ifndef ERELIA_GAME_MODE_H
#define ERELIA_GAME_MODE_H

#include "jgl.h"

#include "erelia_creature.h"

enum class game_mode
{
	editor = 0,
	adventure = 1,
	battle = 2,
	count
};

class Game_mode : public jgl::Widget
{
private:
public:
	Game_mode(jgl::Widget* parent = nullptr) : jgl::Widget(parent)
	{

	};
};

class Editor_mode : public Game_mode
{
private:
	jgl::Contener* _contener;
	Player_controller* _player_controller;
	Interacter* _interacter;
	Editor_inventory* _inventory;
	Editor_interact* _editor_interacter;

public:
	Editor_mode(jgl::Widget* parent = nullptr);

	jgl::Widget* contener() { return (_contener); }
	Editor_inventory* inventory() { return (_inventory); }
	Editor_interact* editor_interacter() { return (_editor_interacter); }
	Player_controller* player_controler(){ return (_player_controller);}
	Interacter* interacter(){ return (_interacter);}

	void active_inventory();
	void desactive_inventory();

	void interact_between(Entity* source, Entity* target);
	void active_interacter();
	void desactive_interacter();

	bool handle_keyboard();

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Adventure_mode : public Game_mode
{
private:
	jgl::Contener* _contener;
	Player_controller* _player_controller;
	Interacter* _interacter;
public:
	Adventure_mode(jgl::Widget* parent = nullptr);

	jgl::Widget* contener() { return (_contener); }

	bool handle_keyboard();

	void interact_between(Entity* source, Entity* target);
	void active_interacter();
	void desactive_interacter();

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

enum class Battle_phase
{
	placement = 0,
	fight = 1,
	exit = 2,
	count
};

class Battle_mode : public Game_mode
{
private:
	jgl::Contener* _contener;
	Battle_arena* _arena;
	Entity* _pointer;

	Battle_phase _phase;

	int _turn_index;
	jgl::Array<Creature_entity*> _turn_order;
	Creature_entity* _active_ally;
	Creature_entity* _active_enemy;

	jgl::Array<Creature_entity*> _allies;
	jgl::Array<Creature_entity*> _enemies;
	jgl::Array<Creature_entity*> _neutrals;


public:
	Battle_mode(jgl::Widget* parent = nullptr);

	void add_creature(Creature_entity* to_add);
	void start(Battle_arena* p_arena, Team_comp first, Team_comp second);
	void exit();

	void place(Creature_entity* entity, jgl::Vector2 pos);

	jgl::Widget* contener() { return (_contener); }
	Battle_arena* arena() { return (_arena); }

	void launch_movement(jgl::Vector2 pos);

	void end_turn();
	void check_movement();
	void change_phase();

	void update();

	bool handle_keyboard_placement();
	bool handle_keyboard_fight();
	bool handle_keyboard();

	bool handle_mouse_placement();
	bool handle_mouse_fight();
	bool handle_mouse();

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif