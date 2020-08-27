#ifndef ERELIA_GAME_MODE_H
#define ERELIA_GAME_MODE_H

#include "jgl.h"

class Game_mode : public jgl::Widget
{
private:
public:
	Game_mode(jgl::Widget* parent = nullptr) : jgl::Widget(parent)
	{

	};

	virtual void enable() = 0;
	virtual void disable() = 0;
};

class Editor_mode : public Game_mode
{
private:
	jgl::Contener* _contener;
	Editor_inventory* _inventory;
	Editor_interact* _interacter;

public:
	Editor_mode(jgl::Widget* parent = nullptr);

	jgl::Widget* contener() { return (_contener); }
	Editor_inventory* inventory() { return (_inventory); }
	Editor_interact* interacter() { return (_interacter); }

	void enable();
	void disable();

	bool handle_keyboard();

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};


#endif