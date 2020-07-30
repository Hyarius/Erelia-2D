#ifndef ERELIA_BOARD_H
#define ERELIA_BOARD_H

#include "erelia_chunk.h"

class Board
{
private:
	std::map<jgl::Vector2, Chunk*> _chunks;

	jgl::Sprite_sheet* _tileset = nullptr;

public:
	Chunk* chunk(jgl::Vector2 pos)
	{
		if (_chunks.contains(pos) == 0)
			return (nullptr);
		return (_chunks[pos]);
	}
	Node* node(jgl::Vector2 pos)
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
	Board(jgl::Sprite_sheet* p_tileset, jgl::String path)
	{
		_tileset = p_tileset;
		load(path);
	}
	~Board()
	{
		for (auto tmp : _chunks)
		{
			delete tmp.second;
		}
	}
	void save(jgl::String path)
	{
		std::fstream file = jgl::open_file(path, std::ios_base::out | std::ios_base::trunc);
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
	void load(jgl::String path)
	{
		bool in_chunk = false;
		jgl::Vector2 chunk_pos;
		int chunk_line;

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
			if (in_chunk == false && tab.size() == 2)
			{
				chunk_pos = jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1]));
				chunk_line = 0;
				in_chunk = true;
			}
			else if (in_chunk == true && tab.size() == chunk_size)
			{
				for (int i = 0; i < chunk_size; i++)
				{
					int type = jgl::stoi(tab[i]);
					if (type != -1 && type < static_cast<int>(node_array.size()))
					{
						jgl::Vector2 node_pos = jgl::Vector2(i, chunk_line) + chunk_pos * chunk_size;
						place(node_pos, node_array[type]);
					}
				}
				chunk_line++;
				if (chunk_line == chunk_size)
					in_chunk = false;
			}
			else
				jgl::error_exit(1, "Error in map file - Line [" + line + "]");
		}
		for (auto tmp : _chunks)
		{
			tmp.second->bake(_tileset);
		}
	}
	jgl::Vector2 tile_on_screen(jgl::Vector2 player_pos, jgl::Vector2 tile_pos)
	{
		jgl::Vector2 result = (tile_pos - player_pos) * node_size + g_application->size() / 2;
		return (result);
	}
	jgl::Vector2 tile_coord(jgl::Vector2 player_pos, jgl::Vector2 coord)
	{
		jgl::Vector2 pos = coord - g_application->size() / 2;
		jgl::Vector2 rel = pos / node_size;

		return ((player_pos + rel).floor());
	}
	jgl::Vector2 chunk_pos(jgl::Vector2 node_pos)
	{
		return ((node_pos / chunk_size).floor());
	}
	jgl::Vector2 rel_node_pos(jgl::Vector2 node_pos)
	{
		jgl::Vector2 tmp = chunk_pos(node_pos);
		return (node_pos - tmp * chunk_size);
	}
	void place(jgl::Vector2 node_pos, size_t index, bool need_bake = false)
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
	void place(jgl::Vector2 node_pos, Node* p_node, bool need_bake = false)
	{
		jgl::Vector2 tmp_chunk = chunk_pos(node_pos);

		if (_chunks.contains(tmp_chunk) == 0)
			_chunks[tmp_chunk] = new Chunk(tmp_chunk);

		Chunk* tmp = _chunks[tmp_chunk];
		tmp->place(rel_node_pos(node_pos), p_node);

		if (need_bake == true)
			tmp->bake(_tileset);
	}
	void bake_chunk(jgl::Vector2 pos)
	{
		if (_chunks.contains(pos) == 0)
			return;
		g_application->viewport()->use();
		_chunks[pos]->bake(_tileset);
	}
	void render(jgl::Vector2 player_pos, jgl::Viewport* viewport)
	{
		jgl::Vector2 delta = jgl::convert_screenV2_to_opengl(player_pos * node_size);
		jgl::Vector2 start = (tile_coord(player_pos, 0) / chunk_size).floor();
		jgl::Vector2 end = (tile_coord(player_pos, g_application->size()) / chunk_size).floor();
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
};

#endif