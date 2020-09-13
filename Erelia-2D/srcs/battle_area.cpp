#include "erelia.h"

Battle_area::Battle_area(jgl::Vector2 p_pos, jgl::Vector2 p_size)
{
	_pos = p_pos;
	_size = p_size;
	glGenBuffers(1, &_vertex_buffer);
	glGenBuffers(1, &_uvs_buffer);
	_points_uvs.clear();
	_points_line.clear();
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
	if (_points_uvs.size() == 0 || _points_line.size() == 0)
	{
		_points_uvs.clear();
		_points_line.clear();
		jgl::Vector2 vtmp = (jgl::Vector2(node_size) / (g_application->size() / 2)) * jgl::Vector2(1, -1);
		for (int i = -_size.x / 2; i < _size.x / 2; i++)
			for (int j = -_size.y / 2; j < _size.y / 2; j++)
			{
				for (size_t h = 0; h < 6; h++)
				{
					jgl::Vector2 value = (jgl::Vector2(i, j) + neightbour[h]) * vtmp;
					_points_uvs.push_back(jgl::Vector3(value.x, value.y, 0.0f));
				}
				for (size_t h = 0; h < 2; h++)
					for (size_t k = 1; k <= 2; k++)
					{
						jgl::Vector2 value = (jgl::Vector2(i, j) + neightbour[h * 3]) * vtmp;
						jgl::Vector2 value2 = (jgl::Vector2(i, j) + neightbour[h * 3 + k]) * vtmp;
						_points_line.push_back(jgl::Vector3(value.x, value.y, 0.0f));
						_points_line.push_back(jgl::Vector3(value2.x, value2.y, 0.0f));
					}
			}
	}
	_uvs.clear();
	for (int i = -_size.x / 2; i < _size.x / 2; i++)
		for (int j = -_size.y / 2; j < _size.y / 2; j++)
		{
			for (size_t h = 0; h < 6; h++)
				_uvs.push_back(-1);
		}
			
}

void Battle_area::render()
{
	const jgl::Vector3* tmp = _points_line.all();

	jgl::draw_line_color(tmp, jgl::Color(0, 0, 0), _points_line.size());
}