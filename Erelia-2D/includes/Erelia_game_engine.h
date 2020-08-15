#ifndef ERELIA_GAME_ENGINE_H
#define ERELIA_GAME_ENGINE_H

extern int node_size;

#include "erelia_board.h"
#include "erelia_player.h"

#include "erelia_renderer.h"
#include "erelia_player_controller.h"
#include "erelia_editor.h"

#include "erelia_prefab.h"
#include "erelia_console.h"

#include "Erelia_interacter.h"

extern std::string custom_texture_shader_vert;
extern std::string custom_texture_shader_frag;

class Game_engine : public jgl::Widget
{
private:
	jgl::Sprite_sheet* _tileset;
	jgl::Sprite_sheet* _charset;
	Board* _board;
	Player* _player;

	Renderer* _renderer;
	jgl::Contener* _editor_contener;
	Player_controller* _player_controller;
	Editor_inventory* _editor_inventory;
	Editor_interact* _editor_interacter;

	Console* _console;

	Interacter* _interacter;

public:
	jgl::Sprite_sheet* tileset() { return (_tileset);}
	jgl::Sprite_sheet* charset() { return (_charset); }
	Board* board() { return (_board); }
	Player* player() { return (_player); }

	Renderer* renderer() { return (_renderer); }
	jgl::Contener* editor_contener() { return (_editor_contener); }
	Player_controller* player_controller() { return (_player_controller); }
	Editor_inventory* editor_inventory() { return (_editor_inventory); }
	Editor_interact* editor_interacter() { return (_editor_interacter); }
	Console* console() { return (_console); }

	Game_engine(jgl::Widget* p_parent = nullptr);
	~Game_engine();

	void active_interacter(Entity* entity);
	void desactive_interacter();
	void active_console();
	void desactive_console();
	void active_inventory();
	void desactive_inventory();

	void save(jgl::String path);
	void load(jgl::String path);

	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

void create_item_list(jgl::Sprite_sheet* tileset);
void delete_item_list();

#endif