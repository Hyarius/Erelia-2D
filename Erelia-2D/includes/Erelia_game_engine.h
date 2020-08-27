#ifndef ERELIA_GAME_ENGINE_H
#define ERELIA_GAME_ENGINE_H

extern int node_size;

#include "erelia_board.h"
#include "erelia_player.h"

#include "erelia_player_controller.h"
#include "erelia_editor.h"

#include "erelia_prefab.h"
#include "erelia_console.h"

#include "Erelia_interacter.h"

#include "erelia_battler.h"

#include "erelia_game_mode.h"

extern std::string custom_texture_shader_vert;
extern std::string custom_texture_shader_frag;

class Game_engine : public jgl::Widget
{
private:
	jgl::Sprite_sheet* _tileset;
	jgl::Sprite_sheet* _charset;
	Board* _board;
	Player* _player;

	Player_controller* _player_controller;
	Console* _console;
	Interacter* _interacter;

	Game_mode* _modes[1];

public:
	jgl::Sprite_sheet* tileset() { return (_tileset);}
	jgl::Sprite_sheet* charset() { return (_charset); }
	Board* board() { return (_board); }
	Player* player() { return (_player); }
	Editor_mode* editor_mode() { return (static_cast<Editor_mode *>(_modes[0])); }
	Game_mode** modes() { return (_modes); }
	Game_mode* modes(size_t index) { if (index >= 1) return (nullptr); return (_modes[index]); }
	Player_controller* player_controller() { return (_player_controller); }
	Console* console() { return (_console); }
	Interacter* interacter() { return (_interacter); }

	Game_engine(jgl::Widget* p_parent = nullptr);
	~Game_engine();

	void interact_between(Entity* source, Entity* target);
	void active_interacter();
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