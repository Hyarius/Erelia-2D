#include "erelia.h"

Game_engine::Game_engine(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_tileset = new jgl::Sprite_sheet("ressources/texture/base_tileset.png", jgl::Vector2(19, 60));
	create_node_item_list(_tileset);
	_board = new Board(_tileset, "ressources/maps/world.map");
	_player = new Player();

	_renderer = new Renderer(_board, _player, this);
	_renderer->activate();

	_player_controller = new Player_controller(_player, this);
	_player_controller->activate();

	_editor_inventory = new Editor_inventory(this);
	_editor_inventory->activate();

	_editor_interacter = new Editor_interact(_editor_inventory, _board, _player, this);
	_editor_interacter->activate();

	_editor_inventory->send_front();
	_renderer->send_back();
}

bool Game_engine::handle_keyboard()
{
	if (jgl::get_key(jgl::key::tab) == jgl::key_state::release)
	{
		if (_editor_inventory->status() == false)
		{
			_player_controller->set_frozen(true);
			_editor_interacter->set_frozen(true);
			_editor_inventory->enable();
		}
		else
		{
			_player_controller->set_frozen(false);
			_editor_interacter->set_frozen(false);
			_editor_inventory->disable();
		}
	}
	return (false);
}


bool Game_engine::handle_mouse()
{
	return (false);
}

void Game_engine::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_renderer->set_geometry(0, p_area);
	_editor_inventory->set_geometry(50, p_area - 100);
	_editor_interacter->set_geometry(0, p_area);
}

void Game_engine::render()
{

}