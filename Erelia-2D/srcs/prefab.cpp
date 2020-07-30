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
	_size = end - start;
	for (float i = start.x; i <= end.x; i++)
		for (float j = start.y; j <= end.y; j++)
		{
			jgl::Vector2 pos = (jgl::Vector2(i, j) - start).floor();
			Node* node = board->node(jgl::Vector2(i, j));
			_content[pos - jgl::Vector2(0.0f, _size.y)] = node;
		}
}

void Prefab::use(Board* board, jgl::Vector2 start)
{
	for (auto tmp : _content)
	{
		jgl::Vector2 pos = tmp.first + start;
		Node* node = tmp.second;
		if (node != nullptr)
			board->place(pos, node);
	}
	jgl::Vector2 A = ((start + jgl::Vector2(_size.x, -_size.y)) / chunk_size).floor();
	jgl::Vector2 B = (start / chunk_size).floor();
	for (float i = A.x; i <= B.x; i++)
		for (float j = A.y; j <= B.y; j++)
		{
			board->bake_chunk(jgl::Vector2(i, j));
		}
}