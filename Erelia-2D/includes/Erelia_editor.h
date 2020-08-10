#ifndef ERELIA_EDITOR_H
#define ERELIA_EDITOR_H

#include "jgl.h"
#include "erelia_item_slot.h"

class Editor_inventory_tab : public jgl::Widget
{
private:
	size_t _nb_slot;
	jgl::Sprite_sheet* _icon_tileset;

	jgl::Frame* _background;

	jgl::Array<Item_slot*> _item_slots;
	jgl::Vscroll_bar* _scroll_bar;

public:
	Editor_inventory_tab(size_t p_nb_slot, jgl::Widget* p_parent = nullptr);
	Item_slot* add_item_slot(Item* p_item);
	Item_slot* find_slot();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Editor_shortcut_bar : public jgl::Widget
{
private:
	int _index;
	jgl::Frame* _background = nullptr;
	Item_slot* _selected = nullptr;
	jgl::Array<Item_slot*> _slots;
public:
	Editor_shortcut_bar(jgl::Widget* p_parent = nullptr);
	void set_item(size_t index, Item* p_item);
	Item_slot* selected() { return (_selected); }
	Item_slot* find_slot();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Editor_mouse_indicator : public jgl::Widget
{
private:
	Item* _item;
public:
	Editor_mouse_indicator(jgl::Widget* p_parent = nullptr);
	void set_item(Item* p_item) { _item = p_item; }
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Editor_inventory : public jgl::Widget
{
private:
	bool _activated = false;
	jgl::Frame* _background = nullptr;
	Editor_mouse_indicator* _indicator = nullptr;
	Editor_shortcut_bar* _shortcut = nullptr;
	Editor_inventory_tab* _active_tab = nullptr;
	jgl::Array<Editor_inventory_tab*> _tab;
	jgl::Array<jgl::Button*> _tab_button;

public:
	Editor_inventory(jgl::Widget* p_parent = nullptr);
	Editor_shortcut_bar* shortcut() { return (_shortcut); }
	Editor_inventory_tab* tab(size_t index) { if (index >= _tab.size())return (nullptr);return (_tab[index]); }
	Item_slot* selected_slot() { return (_shortcut->selected()); }
	Item* select_item();
	bool status();
	void enable();
	void disable();
	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Editor_interact : public jgl::Widget
{
private:
	Editor_inventory* _inventory;
	Entity* _selected_entity;
	Board* _board;
	Player* _player;

	bool _state = false;
	jgl::Vector2 _first = -1;
	jgl::Vector2 _second = -1;
	jgl::Vector2 _pink_flag = -1;
	jgl::Vector2 _blue_flag = -1;

public:
	Editor_interact(Editor_inventory* p_inventory, Board* p_board, Player* p_player, jgl::Widget* p_parent = nullptr);

	jgl::Vector2 pink_flag() { return (_pink_flag); }
	jgl::Vector2 blue_flag() { return (_blue_flag); }

	bool handle_click();
	void handle_remove_multi_pos();
	void handle_multi_pos(Item* tmp);
	bool handle_mouse();
	void update();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif