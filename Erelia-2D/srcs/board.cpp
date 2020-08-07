#include "erelia.h"

Chunk* Board::chunk(jgl::Vector2 pos)
{
	if (_chunks.contains(pos) == 0)
		return (nullptr);
	return (_chunks[pos]);
}
Tile* Board::tile(jgl::Vector2 pos)
{
	Chunk* target_chunk = chunk(chunk_pos(pos));
	if (target_chunk == nullptr)
		return nullptr;
	jgl::Vector2 target_pos = rel_node_pos(pos);
	Node* target_node = target_chunk->node(target_pos);
	if (target_node == nullptr)
		return nullptr;
	return (target_node->tile());
}
Node* Board::node(jgl::Vector2 pos)
{
	Chunk* target_chunk = chunk(chunk_pos(pos));
	if (target_chunk == nullptr)
		return nullptr;
	jgl::Vector2 target_pos = rel_node_pos(pos);
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
	for (auto tmp : _chunks)
	{
		delete tmp.second;
	}
}
void Board::clear()
{
	for (auto tmp : _chunks)
		delete tmp.second;
	_chunks.clear();
}
void Board::save(jgl::String path)
{
	std::fstream file = jgl::open_file(path, std::ios_base::out | std::ios_base::trunc);
	file << "chunks;" << _chunks.size() << std::endl;
	for (auto tmp : _chunks)
	{
		file << tmp.first.x << ";" << tmp.first.y << std::endl;
		for (size_t y = 0; y < chunk_size; y++)
		{
			for (size_t x = 0; x < chunk_size; x++)
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

		for (int nb_line = 0; nb_line < chunk_size; nb_line++)
		{
			jgl::String line = jgl::get_str(file);
			if (line.size() == 0)
				jgl::error_exit(1, "Error in map file - Line [" + line + "]");
			tab = line.split(";");
			for (int i = 0; i < chunk_size; i++)
			{
				int type = jgl::stoi(tab[i]);
				if (type != -1 && type < static_cast<int>(tile_array.size()))
				{
					jgl::Vector2 node_pos = jgl::Vector2(i, nb_line) + chunk_pos * chunk_size;
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
		if (tab.size() < 5)
			jgl::error_exit(1, "Error in map file - Line [" + line + "]");
		jgl::String name = tab[0];
		jgl::Vector2 pos = jgl::Vector2(jgl::stoi(tab[1]), jgl::stoi(tab[2]));
		jgl::Vector2 sprite = jgl::Vector2(jgl::stoi(tab[3]), jgl::stoi(tab[4]));
		NPC* npc = new NPC(name, pos, _charset, sprite);
		_npc_array.push_back(npc);
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
	for (auto tmp : _chunks)
		delete tmp.second;
	_chunks.clear();
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
	return ((node_pos / chunk_size).floor());
}
jgl::Vector2 Board::rel_node_pos(jgl::Vector2 node_pos)
{
	jgl::Vector2 tmp = chunk_pos(node_pos);
	return (node_pos - tmp * chunk_size);
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

void Board::update()
{
	for (size_t i = 0; i < _npc_array.size(); i++)
	{
		_npc_array[i]->update(this);
	}
}

void Board::render(jgl::Vector2 player_pos, jgl::Viewport* viewport)
{
	jgl::Vector2 delta = jgl::convert_screenV2_to_opengl(player_pos * node_size);
	jgl::Vector2 start_node = screen_to_tile(player_pos, 0);
	jgl::Vector2 end_node = screen_to_tile(player_pos, g_application->size());
	jgl::Vector2 start = (start_node / chunk_size).floor();
	jgl::Vector2 end = (end_node / chunk_size).floor();
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			jgl::Vector2 tmp = jgl::Vector2(i, j);
			if (_chunks.contains(tmp) != 0)
				_chunks[tmp]->render(_tileset, delta, viewport);
		}
	for (size_t i = 0; i < _npc_array.size(); i++)
	{
		jgl::Vector2 pos = _npc_array[i]->pos();
		if (point_in_rectangle(pos, start_node, end_node - start_node) == true)
			_npc_array[i]->render(player_pos, viewport);
	}
	for (auto tmp : _warps)
	{
		jgl::Vector2 pos = tile_to_screen(player_pos, tmp.second) + node_size / 2;
		jgl::draw_centred_rectangle(pos, node_size, 1, jgl::Color(146, 235, 52));
		jgl::draw_centred_text(tmp.first, pos - jgl::Vector2(0, 30), 16, 1, jgl::text_color::green);
	}
}