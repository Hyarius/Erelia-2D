#include "erelia.h"

Battle_area::Battle_area(jgl::Vector2 p_pos, jgl::Vector2 p_size)
{
	_pos = p_pos;
	_size = p_size;
	glGenBuffers(1, &_vertex_buffer);
	glGenBuffers(1, &_uvs_buffer);
	_points.clear();
}

void Battle_area::bake()
{
	static jgl::Vector2 neightbour[6] = {
		   jgl::Vector2(0, 0),
		   jgl::Vector2(0, 1),
		   jgl::Vector2(1, 0),
		   jgl::Vector2(0, 1),
		   jgl::Vector2(1, 1),
		   jgl::Vector2(1, 0),
	};
	if (_points.size() == 0)
	{
		jgl::Vector2 vtmp = (jgl::Vector2(node_size) / (g_application->size() / 2)) * jgl::Vector2(1, -1);
		for (int i = -_size.x / 2; i < _size.x / 2; i++)
			for (int j = -_size.y / 2; j < _size.y / 2; j++)
			{
				jgl::Vector2 tmp_pos = jgl::Vector2(i, j) - 0.5f;
				for (size_t h = 0; h < 6; h++)
				{
					jgl::Vector2 value = (tmp_pos + neightbour[h]) * vtmp;
					_points.push_back(jgl::Vector3(value.x, value.y, 0.0f));
				}
			}
	}
	_uvs.clear();
	jgl::Vector2 unit = engine->tileset()->unit();
	for (int i = -_size.x / 2; i < _size.x / 2; i++)
		for (int j = -_size.y / 2; j < _size.y / 2; j++)
		{
			jgl::Vector2 tmp_pos = jgl::Vector2(i, j).round();
			Node* tmp_node = engine->board()->node(_pos + tmp_pos);
			jgl::Vector2 tmp_sprite = engine->tileset()->sprite(tmp_node->tile()->sprite);
			if (tmp_node != nullptr && tmp_node->tile() != nullptr &&
				tmp_node->tile()->type != (OBSTACLE | SWIMABLE))
			{
				for (size_t h = 0; h < 6; h++)
					_uvs.push_back(tmp_sprite + unit * neightbour[h]);
			}
			else
			{
				for (size_t h = 0; h < 6; h++)
					_uvs.push_back(-1);
			}
		}
			
}

void Battle_area::render(jgl::Viewport *p_viewport)
{
	if (p_viewport != nullptr)
		p_viewport->use();

	const jgl::Vector3* tmp = _points.all();
	const jgl::Vector2* tmp2 = _uvs.all();

	jgl::draw_triangle_texture(tmp, tmp2, _points.size());
}