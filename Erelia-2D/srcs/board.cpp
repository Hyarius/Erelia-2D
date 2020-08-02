#include "erelia.h"

Chunk* Board::chunk(jgl::Vector2 pos)
{
	if (_chunks.contains(pos) == 0)
		return (nullptr);
	return (_chunks[pos]);
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
				if (tmp_node == nullptr)
					file << "-1";
				else
					file << tmp_node->index;
			}
			file << std::endl;
		}
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
				if (type != -1 && type < static_cast<int>(node_array.size()))
				{
					jgl::Vector2 node_pos = jgl::Vector2(i, nb_line) + chunk_pos * chunk_size;
					place(node_pos, node_array[type]);
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
		jgl::Array<jgl::String> dialogue;
		for (size_t i = 5; i < tab.size(); i++)
			dialogue.push_back(tab[i]);
		NPC* npc = new NPC(name, dialogue, pos, _charset, sprite);
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
		else
			jgl::error_exit(1, "Error in map file - Line [" + line + "]");
	}
	for (auto tmp : _chunks)
	{
		tmp.second->bake(_tileset);
	}
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
	if (index >= node_array.size())
		place(node_pos, nullptr);
	else
	{
		Node* node = node_array[index];
		place(node_pos, node);
	}
	jgl::Vector2 tmp_chunk = chunk_pos(node_pos);

	if (need_bake == true)
		_chunks[tmp_chunk]->bake(_tileset);

}
void Board::place(jgl::Vector2 node_pos, Node* p_node, bool need_bake)
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
void Board::render(jgl::Vector2 player_pos, jgl::Viewport* viewport)
{
	jgl::Vector2 delta = jgl::convert_screenV2_to_opengl(player_pos * node_size);
	jgl::Vector2 start = (screen_to_tile(player_pos, 0) / chunk_size).floor();
	jgl::Vector2 end = (screen_to_tile(player_pos, g_application->size()) / chunk_size).floor();
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			jgl::Vector2 tmp = jgl::Vector2(i, j);
			if (_chunks.contains(tmp) != 0)
			{
				_chunks[tmp]->render(_tileset, delta, viewport);
			}
		}
}