#ifndef ERELIA_GAME_ENGINE_H
#define ERELIA_GAME_ENGINE_H

extern int node_size;

#include "erelia_board.h"
#include "erelia_player.h"
#include "erelia_creature.h"

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
	jgl::String _save_name;

	jgl::Sprite_sheet* _tileset;
	jgl::Sprite_sheet* _battle_tileset;
	jgl::Sprite_sheet* _charset;
	jgl::Sprite_sheet* _faceset;
	jgl::Sprite_sheet* _back_faceset;
	Board* _board;
	Player* _player;

	Console* _console;

	game_mode _index_mode;
	jgl::Array<Game_mode*> _modes;

public:
	jgl::Sprite_sheet* battle_tileset() { return (_battle_tileset); }
	jgl::Sprite_sheet* tileset() { return (_tileset);}
	jgl::Sprite_sheet* charset() { return (_charset); }
	jgl::Sprite_sheet* faceset() { return (_faceset); }
	jgl::Sprite_sheet* back_faceset() { return (_back_faceset); }
	Board* board() { return (_board); }
	Player* player() { return (_player); }
	Editor_mode* editor_mode() { return (static_cast<Editor_mode*>(_modes[0])); }
	Adventure_mode* adventure_mode() { return (static_cast<Adventure_mode*>(_modes[1])); }
	Battle_mode* battle_mode() { return (static_cast<Battle_mode*>(_modes[2])); }
	game_mode index_mode() { return (_index_mode); }
	jgl::Array<Game_mode*>& modes() { return (_modes); }
	Game_mode* modes(size_t index) { if (index >= _modes.size()) return (nullptr); return (_modes[index]); }
	Game_mode* active_mode() { return (_modes[static_cast<size_t>(_index_mode)]); }
	Console* console() { return (_console); }

	Game_engine(jgl::Widget* p_parent = nullptr);
	~Game_engine();

	void change_mode(game_mode new_mode);

	void active_console();
	void desactive_console();

	void check_encounter();
	void move_player(jgl::Vector2 delta);

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