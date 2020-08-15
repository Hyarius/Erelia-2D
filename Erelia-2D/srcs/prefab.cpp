#include "erelia.h"

Prefab::Prefab()
{
	_content.clear();
	_name = "Unnamed";
}

void Prefab::save(jgl::String p_name, Board* board, jgl::Vector2 _first, jgl::Vector2 _second)
{
	_name = p_name;
	jgl::Vector2 start = jgl::Vector2((_first.x < _second.x ? _first.x : _second.x), (_first.y < _second.y ? _first.y : _second.y));
	jgl::Vector2 end = jgl::Vector2((_first.x > _second.x ? _first.x : _second.x), (_first.y > _second.y ? _first.y : _second.y));
	_size = end - start + 1;
	for (float i = 0; i < _size.x; i++)
		for (float j = 0; j < _size.y; j++)
		{
			jgl::Vector2 pos = (jgl::Vector2(i, j)).floor();
			Tile* tile = board->tile(jgl::Vector2(i, j) + start);
			_content[pos] = tile;
		}
}

Prefab::Prefab(jgl::String path)
{
	std::fstream file = jgl::open_file(path);
	_name = jgl::get_str(file);
	jgl::String line = jgl::get_str(file);
	jgl::Array<jgl::String> tab = line.split(";");
	_size = jgl::Vector2(jgl::stoi(tab[0]), jgl::stoi(tab[1]));
	for (int nb_line = 0; nb_line < _size.y; nb_line++)
	{
		jgl::String line = jgl::get_str(file);
		if (line.size() == 0)
			jgl::error_exit(1, "Error in prefab file - Line [" + line + "]");
		tab = line.split(";");
		for (int i = 0; i < _size.x; i++)
		{
			int type = jgl::stoi(tab[i]);
			if (type < static_cast<int>(tile_array.size()))
			{
				if (type < 0)
					_content[jgl::Vector2(i, nb_line)] = nullptr;
				else
					_content[jgl::Vector2(i, nb_line)] = tile_array[type];
			}
		}
	}
}

void Prefab::save_to_file(jgl::String path)
{
	std::fstream file = jgl::open_file(path, std::ios_base::out | std::ios_base::trunc);
	file << _name << std::endl;
	file << _size.x << ";" << _size.y << std::endl;
	for (int nb_line = 0; nb_line < _size.y; nb_line++)
	{
		for (int i = 0; i < _size.x; i++)
		{
			if (i != 0)
				file << ";";
			if (_content[jgl::Vector2(i, nb_line)] == nullptr)
				file << "-1";
			else
				file << _content[jgl::Vector2(i, nb_line)]->index;
		}
		file << std::endl;
	}
}

void Prefab::use(Board* board, jgl::Vector2 start)
{
	start.y -= _size.y - 1;
	for (auto tmp : _content)
	{
		jgl::Vector2 pos = tmp.first + start;
		Tile* tile = tmp.second;
		if (tile != nullptr)
			board->place(pos, tile);
	}
	jgl::Vector2 A = ((start) / CHUNK_SIZE).floor();
	jgl::Vector2 B = ((start + _size) / CHUNK_SIZE).floor();
	for (float i = A.x; i <= B.x; i++)
		for (float j = A.y; j <= B.y; j++)
		{
			board->bake_chunk(jgl::Vector2(i, j));
		}
}