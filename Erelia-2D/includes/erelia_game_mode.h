#ifndef ERELIA_GAME_MODE_H
#define ERELIA_GAME_MODE_H

#include "jgl.h"

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

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Battle_mode : public Game_mode
{
private:
	jgl::Contener* _contener;
	Battle_area* _arena;
	Entity* _pointer;

public:
	Battle_mode(jgl::Widget* parent = nullptr);

	void start(Battle_area* p_arena);
	void exit();

	jgl::Widget* contener() { return (_contener); }
	Battle_area* arena() { return (_arena); }

	void update();

	bool handle_keyboard();

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif