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

	_editor_contener = new jgl::Contener(this);
	_editor_contener->activate();

	_editor_inventory = new Editor_inventory(_editor_contener);
	_editor_inventory->activate();

	_editor_interacter = new Editor_interact(_editor_inventory, _board, _player, _editor_contener);
	_editor_interacter->activate();

	_console = new Console(_board, _player, this);

	_editor_inventory->send_front();
	_renderer->send_back();
}

extern jgl::Array<Item*> node_item_list;

Game_engine::~Game_engine()
{
	if (_tileset != nullptr)
		delete _tileset;

	if (_board != nullptr)
		delete _board;
	if (_player != nullptr)
		delete _player;

	for (size_t i = 0; i < node_item_list.size(); i++)
	{
		delete node_item_list[i];
	}
	for (size_t i = 0; i < node_array.size(); i++)
	{
		delete node_array[i];
	}
}

bool Game_engine::handle_keyboard()
{
	if (jgl::get_key(jgl::key::F2) == jgl::key_state::release ||
		(jgl::get_key(jgl::key::escape) == jgl::key_state::release && _console->is_active() == true))
	{
		_editor_contener->set_frozen(!_editor_contener->is_frozen());
		_console->set_active(!_console->is_active());
		if (_console->is_active() == true)
			_console->entry()->select();
		_player_controller->set_frozen(!_player_controller->is_frozen());
		_editor_interacter->set_frozen(!_editor_interacter->is_frozen());
		if (_editor_contener->is_frozen() == true)
		{
			_editor_inventory->disable();
			_editor_inventory->shortcut()->desactivate();
		}
		else
			_editor_inventory->shortcut()->activate();
	}
	if (jgl::get_key(jgl::key::tab) == jgl::key_state::release)
	{
		if (_editor_contener->is_frozen() == false)
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

	_console->set_geometry(0, g_application->size());
}

void Game_engine::render()
{

}