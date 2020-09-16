#include "erelia.h"

Battle_area::Battle_area(jgl::Vector2 p_pos, jgl::Vector2 p_size)
{
	_size = p_size;
	_pos = (p_pos - _size / 2.0f).ceiling();
	for (int i = -1; i < _size.x + 1; i++)
	{
		for (int j = -1; j < _size.y + 1; j++)
		{
			jgl::Vector2 tmp = jgl::Vector2(i, j);
			_content[tmp] = new Battle_node(tmp);
			_content[tmp]->type = Battle_node_type::inexistant;
		}
	}
	parse_area(p_pos);
	glGenBuffers(1, &_vertex_buffer);
	glGenBuffers(1, &_uvs_buffer);
	_points.clear();
}

void Battle_area::parse_area(jgl::Vector2 start)
{
	jgl::Array<jgl::Vector2> to_calc;

	for (auto it : _content)
	{
		jgl::Vector2 tmp = it.first;
		jgl::Vector2 target = tmp + _pos;
		Node* tmp_node = engine->board()->node(target);
		if (_content.count(tmp) != 0)
		{
			Battle_node* tmp_bnode = _content[tmp];
			if (is_middle(0, tmp, _size - 1) == true)
			{
				if (tmp_node == nullptr || tmp_node->tile() == nullptr)
				{
					tmp_bnode->type = Battle_node_type::inexistant;
				}
				else if ((tmp_node->tile()->type & SWIMABLE) == SWIMABLE)
				{
					tmp_bnode->calculated = true;
					tmp_bnode->type = Battle_node_type::swimable;
				}
				else if ((tmp_node->tile()->type & WALKABLE) == OBSTACLE || tmp_node->link() != nullptr)
				{
					tmp_bnode->calculated = true;
					tmp_bnode->type = Battle_node_type::obstacle;
				}
				else if (tmp_node->link() == nullptr && tmp_bnode->calculated == false)
				{
					tmp_bnode->calculated = true;
					tmp_bnode->type = Battle_node_type::clear;
				}
			}
			else if (tmp_bnode != nullptr)
			{
				tmp_bnode->calculated = true;
				tmp_bnode->type = Battle_node_type::border;
			}
		}
		
	}

	
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
		for (auto it : _content)
		{
			jgl::Vector2 tmp_pos = it.first - 0.5f;
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
	for (auto it : _content)
	{
		if (it.second != nullptr)
		{
			if (it.second->type != Battle_node_type::inexistant)
			{
				jgl::Vector2 tmp_sprite = engine->battle_tileset()->sprite(jgl::Vector2(static_cast<int>(it.second->type), 0));
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
	}
	const jgl::Vector2* tmp_uvs = _uvs.all();

	glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* _uvs.size() * 2, static_cast<const float*>(&(tmp_uvs[0].x)), GL_STATIC_DRAW);
			
}

void Battle_area::render(jgl::Viewport *p_viewport, jgl::Vector2 base_pos)
{
	if (p_viewport != nullptr)
		p_viewport->use();

	//jgl::Vector2 vtmp = (jgl::Vector2(node_size) / (g_application->size() / 2)) * jgl::Vector2(1, -1);
	jgl::Vector2 delta = jgl::convert_screenV2_to_opengl(tile_to_screen(_pos, base_pos));// *vtmp;

	glBindVertexArray(g_application->vertex_array());

	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, engine->battle_tileset()->image()->texture_id());

	glUniform3f(delta_pos_uniform, delta.x, delta.y, 0.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_points.size()));
}