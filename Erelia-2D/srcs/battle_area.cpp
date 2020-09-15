#include "erelia.h"

Battle_area::Battle_area(jgl::Vector2 p_pos, jgl::Vector2 p_size)
{
	_pos = p_pos;
	_size = p_size;
	jgl::Vector2 tmp_pos = (_pos - _size / 2.0f).ceiling();
	_content = new Battle_node **[static_cast<size_t>(_size.x)];
	for (size_t i = 0; i < _size.x; i++)
	{
		_content[i] = new Battle_node *[static_cast<size_t>(_size.y)];
		for (size_t j = 0; j < _size.y; j++)
		{
			_content[i][j] = new Battle_node(jgl::Vector2(i, j));
			jgl::Vector2 node_pos = tmp_pos + jgl::Vector2(i, j);
			Node* tmp_node = engine->board()->node(node_pos);
			if (tmp_node == nullptr || tmp_node->tile() == nullptr)
				_content[i][j]->type = Battle_node_type::inexistant;
			else if (tmp_node->tile()->type == OBSTACLE || tmp_node->tile()->type == SWIMABLE || tmp_node->link() != nullptr)
				_content[i][j]->type = Battle_node_type::obstacle;
			else
				_content[i][j]->type = Battle_node_type::clear;
		}
	}
	glGenBuffers(1, &_vertex_buffer);
	glGenBuffers(1, &_uvs_buffer);
	_points.clear();
}

void Battle_area::rebake()
{
	_points.clear();
	bake();
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
		for (int i = -_size.x / 2 - 1; i < _size.x / 2 + 1; i++)
			for (int j = -_size.y / 2 - 1; j < _size.y / 2 + 1; j++)
			{
				jgl::Vector2 tmp_pos = jgl::Vector2(i, j) - 0.5f;
				for (size_t h = 0; h < 6; h++)
				{
					jgl::Vector2 value = (tmp_pos + neightbour[h]) * vtmp;
					_points.push_back(jgl::Vector3(value.x, value.y, 0.0f));
				}
			}
		const jgl::Vector3* tmp = _points.all();
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _points.size() * 3, static_cast<const float*>(&(tmp[0].x)), GL_STATIC_DRAW);
	}
	_uvs.clear();
	jgl::Vector2 unit = engine->battle_tileset()->unit();
	for (int i = -1; i < _size.x + 1; i++)
		for (int j = -1; j < _size.y + 1; j++)
		{
			if (i != -1 && j != -1 && i < _size.x && j < _size.y && _content[i][j]->type != Battle_node_type::inexistant)
			{
				jgl::Vector2 tmp_sprite = engine->battle_tileset()->sprite(jgl::Vector2(static_cast<int>(_content[i][j]->type), 0));
				for (size_t h = 0; h < 6; h++)
				{
					jgl::Vector2 tmp_value = tmp_sprite + unit * neightbour[h];
					_uvs.push_back(tmp_value);
				}
			}
			else if (i == -1 || j == -1 || i == _size.x || j == _size.y)
			{
				jgl::Vector2 tmp_sprite = engine->battle_tileset()->sprite(jgl::Vector2(1, 0));
				for (size_t h = 0; h < 6; h++)
				{
					jgl::Vector2 tmp_value = tmp_sprite + unit * neightbour[h];
					_uvs.push_back(tmp_value);
				}
			}
			else
			{
				for (size_t h = 0; h < 6; h++)
					_uvs.push_back(-1);
			}
		}
	const jgl::Vector2* tmp_uvs = _uvs.all();

	glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* _uvs.size() * 2, static_cast<const float*>(&(tmp_uvs[0].x)), GL_STATIC_DRAW);
			
}

void Battle_area::render(jgl::Viewport *p_viewport)
{
	if (p_viewport != nullptr)
		p_viewport->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, engine->battle_tileset()->image()->texture_id());

	jgl::draw_triangle_texture(_vertex_buffer, _uvs_buffer, 1, _points.size());
}