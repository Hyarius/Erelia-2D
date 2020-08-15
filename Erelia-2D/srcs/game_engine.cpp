#include "erelia.h"

Game_engine::Game_engine(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_tileset = new jgl::Sprite_sheet("ressources/texture/base_tileset.png", jgl::Vector2(19, 72));
	_charset = new jgl::Sprite_sheet("ressources/texture/charset.png", jgl::Vector2(10, 41));
	create_item_list(_tileset);
	_board = new Board(_tileset, _charset, "ressources/maps/world.map");
	_player = new Player(_charset, 0);
	if (jgl::check_file_exist("ressources/save/Player.sav") == true)
		load("ressources/save/Player.sav");

	_renderer = new Renderer(_board, _player, this);
	_renderer->activate();

	_player_controller = new Player_controller(_board, _player, this);
	_player_controller->activate();

	_editor_contener = new jgl::Contener(this);
	_editor_contener->activate();

	_editor_inventory = new Editor_inventory(_editor_contener);
	_editor_inventory->activate();

	_editor_interacter = new Editor_interact(_editor_inventory, _board, _player, _editor_contener);
	_editor_interacter->activate();

	_console = new Console(this);

	_interacter = new Interacter(this);

	_editor_inventory->send_front();
	_renderer->send_back();
}

extern jgl::Array<Item*> node_item_list;

Game_engine::~Game_engine()
{
	if (_tileset != nullptr)
		delete _tileset;
	if (_charset != nullptr)
		delete _charset;

	if (_board != nullptr)
		delete _board;
	if (_player != nullptr)
		delete _player;

	delete_item_list();
	for (size_t i = 0; i < tile_array.size(); i++)
	{
		delete tile_array[i];
	}
	for (size_t i = 0; i < prefab_array.size(); i++)
	{
		delete prefab_array[i];
	}
}

void Game_engine::save(jgl::String path)
{
	std::fstream file = jgl::open_file(path, std::ios_base::out | std::ios_base::trunc);

	file << _player->name() << std::endl;
	file << _player->pos().x << ";" << _player->pos().y << std::endl;
}

void Game_engine::load(jgl::String path)
{
	std::fstream file = jgl::open_file(path, std::ios_base::in);

	jgl::String line = jgl::get_str(file);
	_player->set_name(line);
	jgl::Array<jgl::String> tab = jgl::get_strsplit(file, ";");
	_player->place(_board, jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1])).floor());
}

void Game_engine::active_interacter(Entity* entity)
{
	_interacter->set_entity(entity);

}

void Game_engine::desactive_interacter()
{

}

void Game_engine::active_console()
{
	_editor_contener->set_frozen(true);
	_console->start();
	_player_controller->set_frozen(true);
	_editor_interacter->set_frozen(true);
	_editor_inventory->desactivate();
}

void Game_engine::desactive_console()
{
	_editor_contener->set_frozen(false);
	_console->desactivate();
	_console->entry()->select();
	_player_controller->set_frozen(false);
	_editor_interacter->set_frozen(false);
	_editor_inventory->activate();
	desactive_inventory();
}

void Game_engine::active_inventory()
{
	_player_controller->set_frozen(true);
	_editor_interacter->set_frozen(true);
	_editor_inventory->enable();
}

void Game_engine::desactive_inventory()
{
	_player_controller->set_frozen(false);
	_editor_interacter->set_frozen(false);
	_editor_inventory->disable();
}

void Game_engine::update()
{
	_board->update(_player->pos());
}

bool Game_engine::handle_keyboard()
{
	if (jgl::get_key(jgl::key::F2) == jgl::key_state::release ||
		(jgl::get_key(jgl::key::escape) == jgl::key_state::release && _console->is_active() == true))
	{
		if (_console->is_active() == false)
			active_console();
		else
			desactive_console();
	}
	if (_console->is_active() == true && _console->complete() == true)
		desactive_console();
	if (jgl::get_key(jgl::key::tab) == jgl::key_state::release || (_editor_inventory->status() == true && jgl::get_key(jgl::key::escape) == jgl::key_state::release))
	{
		if (_editor_contener->is_frozen() == false)
		{
			if (_editor_inventory->status() == false)
				active_inventory();
			else
				desactive_inventory();
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
	int tmp = g_application->size().x / 30;
	_editor_inventory->set_geometry(tmp, p_area - tmp * 2);
	_editor_interacter->set_geometry(0, p_area);

	_console->set_geometry(0, g_application->size());
	_interacter->set_geometry(0, g_application->size());
}

void Game_engine::render()
{

}