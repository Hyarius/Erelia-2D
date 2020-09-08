#include "erelia.h"

Game_engine::Game_engine(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	engine = this;
	_tileset = new jgl::Sprite_sheet("ressources/texture/base_tileset.png", jgl::Vector2(19, 143));
	_charset = new jgl::Sprite_sheet("ressources/texture/charset.png", jgl::Vector2(10, 42));
	_faceset = new jgl::Sprite_sheet("ressources/texture/face.png", jgl::Vector2(16, 10));
	create_item_list(_tileset);
	create_creature_list(_faceset);
	_board = new Board("ressources/maps/world.map");
	_player = new Player();
	if (jgl::check_file_exist("ressources/save/Player.sav") == true)
		load("ressources/save/Player.sav");

	_player_controller = new Player_controller(this);
	_player_controller->activate();

	_modes[0] = new Editor_mode(this);
	_modes[0]->activate();

	_console = new Console(this);

	_interacter = new Interacter(this);

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

void Game_engine::interact_between(Entity* source, Entity* target)
{
	if (source == nullptr || target == nullptr)
		return;
	if (target->look_dir() == Entity_direction::south)
		source->set_look_dir(Entity_direction::north);
	else if (target->look_dir() == Entity_direction::north)
		source->set_look_dir(Entity_direction::south);
	else if (target->look_dir() == Entity_direction::east)
		source->set_look_dir(Entity_direction::west);
	else if (target->look_dir() == Entity_direction::west)
		source->set_look_dir(Entity_direction::east);

	if (source->interaction().size() == 0)
		return ;
	
	_interacter->set_entity(source, target);

	active_interacter();
}

void Game_engine::move_player(jgl::Vector2 delta)
{
	_player->move(delta);
	if (_board->node(_player->pos() + delta) != nullptr && _board->node(_player->pos() + delta)->tile() != nullptr &&
		_board->node(_player->pos() + delta)->encounter_area() != nullptr)
	{
		Battle_data* tmp = _board->node(_player->pos() + delta)->encounter_area();
		Encounter_data result = tmp->ask();
		if (result != Encounter_data::null())
		{
			std::cout << "BOUM ! COMBAT WITH ENTITY " << result.id << " !" << std::endl;
		}
	}
}

void Game_engine::active_interacter()
{
	static_cast<Editor_mode*>(_modes[0])->contener()->desactivate();
	_player_controller->set_frozen(true);
	_interacter->activate();
	_interacter->run_action();
}

void Game_engine::desactive_interacter()
{
	static_cast<Editor_mode*>(_modes[0])->contener()->activate();
	_player_controller->set_frozen(false);
	_interacter->activate();
	_interacter->set_entity(nullptr, nullptr);
}

void Game_engine::active_console()
{
	static_cast<Editor_mode*>(_modes[0])->contener()->set_frozen(true);
	_console->enable();
	_player_controller->set_frozen(true);
	static_cast<Editor_mode*>(_modes[0])->interacter()->set_frozen(true);
	static_cast<Editor_mode*>(_modes[0])->inventory()->desactivate();
}

void Game_engine::desactive_console()
{
	static_cast<Editor_mode*>(_modes[0])->contener()->set_frozen(false);
	_console->disable();
	_player_controller->set_frozen(false);
	static_cast<Editor_mode*>(_modes[0])->interacter()->set_frozen(false);
	static_cast<Editor_mode*>(_modes[0])->inventory()->activate();
	desactive_inventory();
}

void Game_engine::active_inventory()
{
	_player_controller->set_frozen(true);
	_modes[0]->enable();
}

void Game_engine::desactive_inventory()
{
	_player_controller->set_frozen(false);
	_modes[0]->disable();
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
		if (_console->entry()->is_active() == false)
			active_console();
		else
			desactive_console();
	}
	if (jgl::get_key(jgl::key::F1) == jgl::key_state::release)
	{
		node_size /= 2;
		_board->bake();
	}
	else if (jgl::get_key(jgl::key::F3) == jgl::key_state::release)
	{
		node_size *= 2;
		_board->bake();
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
	_modes[0]->set_geometry(p_anchor, p_area);
	_interacter->set_geometry(0, g_application->size());
	_console->set_geometry(0, g_application->size());
}

void Game_engine::render()
{
	_board->render(_viewport);
	_player->render(_viewport);
	jgl::draw_text("Fps : " + jgl::itoa(print_fps), 50, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, _viewport);
}