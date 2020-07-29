#ifndef ERELIA_GAME_ENGINE_H
#define ERELIA_GAME_ENGINE_H

#include "erelia_board.h"
#include "erelia_player.h"

#include "erelia_renderer.h"
#include "erelia_player_controller.h"
#include "erelia_editor.h"

#include "erelia_console.h"

extern std::string custom_texture_shader_vert;
extern std::string custom_texture_shader_frag;

class Game_engine : public jgl::Widget
{
private:
	jgl::Sprite_sheet* _tileset;
	Board* _board;
	Player* _player;

	Renderer* _renderer;
	jgl::Contener* _editor_contener;
	Player_controller* _player_controller;
	Editor_inventory* _editor_inventory;
	Editor_interact* _editor_interacter;

	Console* _console;

public:
	Game_engine(jgl::Widget* p_parent = nullptr);
	~Game_engine();
	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

void create_node_item_list(jgl::Sprite_sheet* tileset);

#endif