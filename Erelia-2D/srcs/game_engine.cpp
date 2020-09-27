#include "erelia.h"

Game_engine::Game_engine(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	engine = this;
	_tileset = new jgl::Sprite_sheet("ressources/texture/base_tileset.png", jgl::Vector2(19, 143));
	_battle_tileset = new jgl::Sprite_sheet("ressources/texture/battle_tile.png", jgl::Vector2(16, 1));
	_charset = new jgl::Sprite_sheet("ressources/texture/charset.png", jgl::Vector2(10, 42));
	_faceset = new jgl::Sprite_sheet("ressources/texture/face.png", jgl::Vector2(16, 10));
	_back_faceset = new jgl::Sprite_sheet("ressources/texture/back_face.png", jgl::Vector2(16, 10));
	create_item_list(_tileset);
	create_creature_list(_faceset);
	_board = new Board("ressources/maps/world.map");
	_player = new Player();
	if (jgl::check_file_exist("ressources/save/Player.sav") == true)
		load("ressources/save/Player.sav");

	_index_mode = game_mode::editor;
	_modes.push_back(new Editor_mode(this));
	_modes.push_back(new Adventure_mode(this));
	_modes.push_back(new Battle_mode(this));

	change_mode(game_mode::adventure);

	_console = new Console(this);

	_console->send_front();
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
	file << _player->pos().round().x << ";" << _player->pos().round().y << std::endl;
}

void Game_engine::load(jgl::String path)
{
	std::fstream file = jgl::open_file(path, std::ios_base::in);

	jgl::String line = jgl::get_str(file);
	_player->set_name(line);
	jgl::Array<jgl::String> tab = jgl::get_strsplit(file, ";");
	_player->place(jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1])).floor());
}

void Game_engine::check_encounter()
{
	if (_index_mode == game_mode::adventure &&
		_board->node(_player->pos()) != nullptr &&
		_board->node(_player->pos())->encounter_area() != nullptr)
	{
		Battle_data* tmp = _board->node(_player->pos())->encounter_area();
		Encounter_data result = tmp->ask();
		if (result != Encounter_data::null())
		{
			std::cout << "BOUM ! COMBAT WITH ENTITY " << result.id << " !" << std::endl;
			Battle_arena* new_area = new Battle_arena(_player->pos(), jgl::Vector2(13, 9));
			change_mode(game_mode::battle);
			Team_comp ally = Team_comp({});
			for (size_t i = 0; i < 3; i++)
				ally.define(i, new Creature_entity(creature_list[0], Team::ally));
			Team_comp enemy = Team_comp({});
			for (size_t i = 0; i < 6; i++)
				enemy.define(i, new Creature_entity(creature_list[result.id], Team::enemy));
			battle_mode()->start(new_area, ally, enemy);
			_board->node(_player->pos())->set_occupant(nullptr);
			_player->place(_player->pos());
		}
	}
}

void Game_engine::move_player(jgl::Vector2 delta)
{
	_player->move(delta);
}

void Game_engine::active_console()
{
	_console->enable();
	_modes[static_cast<size_t>(_index_mode)]->set_frozen(true);
}

void Game_engine::desactive_console()
{
	_console->disable();
	_modes[static_cast<size_t>(_index_mode)]->set_frozen(false);
}

void Game_engine::change_mode(game_mode new_mode)
{
	size_t tmp = static_cast<size_t>(new_mode);
	if (_modes.size() > tmp && _modes[tmp] != nullptr)
	{
		_index_mode = new_mode;
		for (size_t i = 0; i < _modes.size(); i++)
		{
			if (_modes[i] != nullptr)
				_modes[i]->desactivate();
		}
		_modes[static_cast<size_t>(_index_mode)]->activate();
	}
	
}

void Game_engine::update()
{
	_board->update();
}

bool Game_engine::handle_keyboard()
{
	if (jgl::get_key(jgl::key::F2) == jgl::key_state::release ||
		(jgl::get_key(jgl::key::escape) == jgl::key_state::release && _console->is_active() == true))
	{
		std::cout << "Here ?" << std::endl;
		if (_console->entry()->is_active() == false)
			active_console();
		else
			desactive_console();
	}
	if (jgl::get_key(jgl::key::F1) == jgl::key_state::release)
	{
		node_size /= 2;
		_board->bake();
		if (battle_mode()->arena() != nullptr)
			battle_mode()->arena()->rebake(battle_mode()->arena_renderer()->viewport());

	}
	else if (jgl::get_key(jgl::key::F3) == jgl::key_state::release)
	{
		node_size *= 2;
		_board->bake();
		if (battle_mode()->arena() != nullptr)
			battle_mode()->arena()->rebake(battle_mode()->arena_renderer()->viewport());
	}
	else if (jgl::get_key(jgl::key::F10) == jgl::key_state::release)
	{
		_modes[0]->set_active(!(_modes[0]->is_active()));
	}
	if (_console->is_active() == true && _console->complete() == true)
		desactive_console();
	return (false);
}


bool Game_engine::handle_mouse()
{
	return (false);
}

void Game_engine::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	for (size_t i = 0; i < _modes.size(); i++)
		_modes[i]->set_geometry(p_anchor, p_area);
	_console->set_geometry(0, g_application->size());
}

void Game_engine::render()
{
	
}