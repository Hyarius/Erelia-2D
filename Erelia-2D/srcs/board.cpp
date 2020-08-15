#include "erelia.h"

Chunk* Board::chunk(jgl::Vector2 pos)
{
	if (_chunks.contains(pos) == 0)
		return (nullptr);
	return (_chunks[pos]);
}
Tile* Board::tile(jgl::Vector2 pos)
{
	Chunk* target_chunk = chunk(chunk_pos(pos.round()));
	if (target_chunk == nullptr)
		return nullptr;
	jgl::Vector2 target_pos = rel_node_pos(pos.round());
	Node* target_node = target_chunk->node(target_pos);
	if (target_node == nullptr)
		return nullptr;
	return (target_node->tile());
}
Node* Board::node(jgl::Vector2 pos)
{
	Chunk* target_chunk = chunk(chunk_pos(pos.round()));
	if (target_chunk == nullptr)
		return nullptr;
	jgl::Vector2 target_pos = rel_node_pos(pos.round());
	Node* target_node = target_chunk->node(target_pos);
	if (target_node == nullptr)
		return nullptr;
	return (target_node);
}
Board::Board(jgl::Sprite_sheet* p_tileset, jgl::Sprite_sheet* p_charset, jgl::String path)
{
	_tileset = p_tileset;
	_charset = p_charset;
	load(path);
}
Board::~Board()
{
	clear();
}
void Board::clear()
{
	for (auto tmp : _chunks)
		delete tmp.second;
	_chunks.clear();
	_warps.clear();
	for (size_t i = 0; i < _npc_array.size(); i++)
		delete _npc_array[i];
	for (size_t i = 0; i < _links.size(); i++)
		delete _links[i];
}
void Board::save(jgl::String path)
{
	std::fstream file = jgl::open_file(path, std::ios_base::out | std::ios_base::trunc);
	file << "chunks;" << _chunks.size() << std::endl;
	for (auto tmp : _chunks)
	{
		file << tmp.first.x << ";" << tmp.first.y << std::endl;
		for (size_t y = 0; y < CHUNK_SIZE; y++)
		{
			for (size_t x = 0; x < CHUNK_SIZE; x++)
			{
				if (x != 0)
					file << ";";
				Node* tmp_node = tmp.second->node(jgl::Vector2(x, y));
				if (tmp_node == nullptr || tmp_node->tile() == nullptr)
					file << "-1";
				else
					file << tmp_node->tile()->index;
			}
			file << std::endl;
		}
	}
	file << "npc;" << _npc_array.size() << std::endl;
	for (size_t i = 0; i < _npc_array.size(); i++)
	{
		_npc_array[i]->save(file);
	}
	file << "warp;" << _warps.size() << std::endl;
	for (auto tmp : _warps)
	{
		file << tmp.first << ";" << tmp.second.x << ";" << tmp.second.y << std::endl;
	}
	file << "link;" << _links.size() << std::endl;
	for (size_t i = 0; i < _links.size(); i++)
	{
		file << _links[i]->a().x << ";" << _links[i]->a().y << ";" << _links[i]->b().x << ";" << _links[i]->b().y << ";";
		if (node(_links[i]->a())->link() == node(_links[i]->b())->link())
			file << "1" << std::endl;
		else
			file << "0" << std::endl;
	}
}

void Board::load_warp(std::fstream& file, jgl::Array<jgl::String> tab)
{
	size_t nb_warp = jgl::stoi(tab[1]);
	for (size_t t = 0; t < nb_warp; t++)
	{
		jgl::String line = jgl::get_str(file);
		if (line.size() == 0)
			break;
		jgl::Array<jgl::String> tab = line.split(";");
		if (tab.size() != 3)
			jgl::error_exit(1, "Error in map file - Line [" + line + "]");
		jgl::String name = tab[0];
		jgl::Vector2 pos = jgl::Vector2(jgl::stoi(tab[1]), jgl::stoi(tab[2])).floor();
		add_warp(name, pos);
	}
}

void Board::load_chunk(std::fstream& file, jgl::Array<jgl::String> tab)
{
	size_t nb_chunk = jgl::stoi(tab[1]);
	bool in_chunk = false;
	jgl::Vector2 chunk_pos;
	for (size_t t = 0; t < nb_chunk; t++)
	{
		jgl::String line = jgl::get_str(file);
		if (line.size() == 0)
			break;
		tab = line.split(";");
		chunk_pos = jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1]));

		for (int nb_line = 0; nb_line < CHUNK_SIZE; nb_line++)
		{
			jgl::String line = jgl::get_str(file);
			if (line.size() == 0)
				jgl::error_exit(1, "Error in map file - Line [" + line + "]");
			tab = line.split(";");
			for (int i = 0; i < CHUNK_SIZE; i++)
			{
				int type = jgl::stoi(tab[i]);
				if (type != -1 && type < static_cast<int>(tile_array.size()))
				{
					jgl::Vector2 node_pos = jgl::Vector2(i, nb_line) + chunk_pos * CHUNK_SIZE;
					place(node_pos, tile_array[type]);
				}
			}
		}
	}
}

void Board::load_npc(std::fstream& file, jgl::Array<jgl::String> tab)
{
	size_t nb_npc = jgl::stoi(tab[1]);
	for (size_t t = 0; t < nb_npc; t++)
	{
		jgl::String line = jgl::get_str(file);
		if (line.size() == 0)
			break;
		jgl::Array<jgl::String> tab = line.split(";");
		if (tab.size() < 5 || tab.size() % 2 == 0)
			jgl::error_exit(1, "Error in map file - Line [" + line + "]");
		jgl::String name = tab[0];
		jgl::Vector2 pos = jgl::Vector2(jgl::stoi(tab[1]), jgl::stoi(tab[2]));
		jgl::Vector2 sprite = jgl::Vector2(jgl::stoi(tab[3]), jgl::stoi(tab[4]));
		NPC* npc = new NPC(name, pos, _charset, sprite);
		for (size_t i = 5; i < tab.size(); i += 2)
			npc->add_check_point(jgl::Vector2(jgl::stoi(tab[i]), jgl::stoi(tab[i + 1])));
		add_npc(npc);
	}
}

void Board::load_link(std::fstream& file, jgl::Array<jgl::String> tab)
{
	size_t nb_link = jgl::stoi(tab[1]);
	for (size_t t = 0; t < nb_link; t++)
	{
		jgl::String line = jgl::get_str(file);
		if (line.size() == 0)
			break;
		jgl::Array<jgl::String> tab = line.split(";");
		if (tab.size() < 4)
			jgl::error_exit(1, "Error in map file - Line [" + line + "]");
		jgl::Vector2 a = jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1]));
		jgl::Vector2 b = jgl::Vector2(jgl::stoi(tab[2]), jgl::stoi(tab[3]));
		bool tmp = (tab[4] == "1");
		add_link(a, b, tmp);
	}
}

void Board::load(jgl::String path)
{
	clear();
	
	std::fstream file = jgl::open_file(path);
	while (file.eof() == false)
	{
		jgl::String line = jgl::get_str(file);
		if (line.size() == 0)
			break;
		jgl::Array<jgl::String> tab = line.split(";");
		if (tab[0] == "npc")
		{
			load_npc(file, tab);
		}
		else if (tab[0] == "chunks")
		{
			load_chunk(file, tab);
		}
		else if (tab[0] == "warp")
		{
			load_warp(file, tab);
		}
		else if (tab[0] == "link")
		{
			load_link(file, tab);
		}
		else
			jgl::error_exit(1, "Error in map file - Line [" + line + "]");
	}
	for (auto tmp : _chunks)
	{
		tmp.second->bake(_tileset);
	}
}

void Board::add_warp(jgl::String name, jgl::Vector2 pos)
{
	_warps[name.std()] = pos;
}

void Board::add_link(jgl::Vector2 p_a, jgl::Vector2 p_b, bool dual)
{
	Node* a = node(p_a);
	Node* b = node(p_b);

	if (a == nullptr || b == nullptr)
		return;
	Link* tmp = new Link(p_a, p_b);
	_links.push_back(tmp);
	a->set_link(tmp);
	if (dual == true)
		b->set_link(tmp);
}

void Board::add_npc(NPC* p_npc)
{
	_npc_array.push_back(p_npc);
	node(p_npc->pos())->set_occupant(p_npc);
}

void Board::remove_npc(jgl::String p_name)
{
	for (size_t i = 0; i < _npc_array.size(); i++)
	{
		if (_npc_array[i]->name() == p_name)
		{
			delete _npc_array[i];
			_npc_array.erase(i);
		}
	}
}


void Board::remove_link(jgl::Vector2 pos)
{
	Node* a = node(pos);
	if (a != nullptr && a->link() != nullptr)
	{
		Link* link = a->link();
		a = node(link->a());
		Node* b = node(link->b());
		a->set_link(nullptr);
		b->set_link(nullptr);
		if (_links.find(link) != _links.end())
		{
			_links.erase(_links.find(link));
		}
		delete link;
	}
}

void Board::remove_warp(jgl::String name)
{
	_warps.erase(name.std());
}

jgl::Vector2 Board::chunk_pos(jgl::Vector2 node_pos)
{
return ((node_pos / CHUNK_SIZE).floor());
}
jgl::Vector2 Board::rel_node_pos(jgl::Vector2 node_pos)
{
	jgl::Vector2 tmp = chunk_pos(node_pos.round());
	return (node_pos - tmp * CHUNK_SIZE);
}
void Board::place(jgl::Vector2 node_pos, size_t index, bool need_bake)
{
	if (index >= tile_array.size())
		place(node_pos, nullptr);
	else
	{
		Tile* tile = tile_array[index];
		place(node_pos, tile);
	}
	jgl::Vector2 tmp_chunk = chunk_pos(node_pos);

	if (need_bake == true)
		_chunks[tmp_chunk]->bake(_tileset);

}
void Board::place(jgl::Vector2 node_pos, Tile* p_node, bool need_bake)
{
	jgl::Vector2 tmp_chunk = chunk_pos(node_pos);

	if (_chunks.contains(tmp_chunk) == 0)
		_chunks[tmp_chunk] = new Chunk(tmp_chunk);

	Chunk* tmp = _chunks[tmp_chunk];
	tmp->place(rel_node_pos(node_pos), p_node);

	if (need_bake == true)
		tmp->bake(_tileset);
}
void Board::bake_chunk(jgl::Vector2 pos)
{
	if (_chunks.contains(pos) == 0)
		return;
	g_application->viewport()->use();
	_chunks[pos]->bake(_tileset);
}

void Board::update(jgl::Vector2 player_pos)
{
	jgl::Vector2 start_node = screen_to_tile(player_pos, 0);
	jgl::Vector2 end_node = screen_to_tile(player_pos, g_application->size());
	jgl::Vector2 start = (start_node / CHUNK_SIZE).floor();
	jgl::Vector2 end = (end_node / CHUNK_SIZE).floor();
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			jgl::Vector2 tmp = jgl::Vector2(i, j);
			if (_chunks.contains(tmp) != 0)
				_chunks[tmp]->update(this);
		}
}

void Board::render(jgl::Vector2 player_pos, jgl::Viewport* viewport)
{
	jgl::Vector2 start_node = screen_to_tile(player_pos, 0);
	jgl::Vector2 end_node = screen_to_tile(player_pos, g_application->size());
	jgl::Vector2 start = (start_node / CHUNK_SIZE).floor();
	jgl::Vector2 end = (end_node / CHUNK_SIZE).floor();
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			jgl::Vector2 tmp = jgl::Vector2(i, j);
			if (_chunks.contains(tmp) != 0)
				_chunks[tmp]->render(_tileset, player_pos, viewport);
		}
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			jgl::Vector2 tmp = jgl::Vector2(i, j);
			if (_chunks.contains(tmp) != 0)
				_chunks[tmp]->render_entity(player_pos, viewport);
		}
	for (auto tmp : _warps)
	{
		jgl::Vector2 pos = tile_to_screen(player_pos, tmp.second) + node_size / 2;
		jgl::draw_centred_rectangle(pos, node_size, 1, jgl::Color(146, 235, 52));
		jgl::draw_centred_text(tmp.first, pos - jgl::Vector2(0, 30), 16, 1, jgl::text_color::green);
	}
}

bool Board::can_acces(jgl::Vector2 pos, jgl::Vector2 delta)
{
	jgl::Vector2 norm_delta = delta.normalize().round();
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };
	static node_type rev_type[4] = { DOWN_WALKABLE, UP_WALKABLE, LEFT_WALKABLE, RIGHT_WALKABLE };
	static node_type type[4] = { UP_WALKABLE, DOWN_WALKABLE, RIGHT_WALKABLE, LEFT_WALKABLE };
	size_t i = 0;
	for (; i < 4; i++)
		if (norm_delta == move_delta[i])
			break;
	if (i == 4)
		return (false);
	Node* tmp = node((pos + delta).round());
	Node* actual = node(pos.round());
	if (tmp != nullptr && actual != nullptr &&
		tmp->tile() != nullptr && actual->tile() != nullptr &&
		(tmp->tile()->type & rev_type[i]) == rev_type[i] &&
		(actual->tile()->type & type[i]) == type[i])
		{
			if ((tmp->tile()->type & JUMPING) == JUMPING)
			{
				return (can_acces(pos, delta + norm_delta));
			}
			else
				return (true);
		}
	return (false);
}

Node* Board::find_closest(jgl::Array<jgl::Vector2>& to_calc)
{
	Node* result = nullptr;

	for (size_t i = 0; i < to_calc.size(); i++)
	{
		Node* other = node(to_calc[i]);
		if (other != nullptr && other->calculated() == false && (result == nullptr || (result->t_cost() > other->t_cost())))
			result = other;
	}
	return (result);
}

jgl::Array<jgl::Vector2> Board::pathfinding(jgl::Vector2 start, jgl::Vector2 end)
{
	if (start == end ||
		node(start) == nullptr || node(start)->tile() == nullptr || node(start)->tile()->type != WALKABLE ||
		node(end) == nullptr || node(end)->tile() == nullptr || node(end)->tile()->type != WALKABLE)
		return (jgl::Array<jgl::Vector2>());
	static jgl::Vector2 neightbour[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };

	jgl::Array<jgl::Vector2> result;
	jgl::Array<jgl::Vector2> to_calc;
	
	node(start)->calc_cost(0, end);
	if (node(start)->t_cost() >= 30)
		return (jgl::Array<jgl::Vector2>());
	to_calc.push_back(start);
	Node* target_node = node(start);
	target_node->set_parent(nullptr);
	while (target_node != nullptr)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (can_acces(target_node->pos(), neightbour[j]) == true)
			{
				jgl::Vector2 actual = target_node->pos() + neightbour[j];
				Node* actual_node = node(actual);
				if (actual_node != nullptr && actual_node->calculated() == false)
				{
					node(actual)->calc_cost(target_node->s_cost() + 1, end);
					if (node(actual)->t_cost() > 30)
						return (jgl::Array<jgl::Vector2>());
					node(actual)->set_parent(target_node);
					to_calc.push_back(actual);
				}
			}
		}
		target_node->set_calculated(true);
		target_node = find_closest(to_calc);
		
		if (target_node != nullptr && target_node->pos() == end)
			target_node = nullptr;
	}
	for (size_t i = 0; i < to_calc.size(); i++)
		node(to_calc[i])->set_calculated(false);
	target_node = node(end);
	while (target_node != nullptr)
	{
		result.push_back(target_node->pos());
		target_node = target_node->parent();
	}
	result.reverse();
	return (result);
}