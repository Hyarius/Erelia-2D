#include "erelia.h"

Battle_arena::Battle_arena(jgl::Vector2 p_pos, jgl::Vector2 p_size)
{
	_size = p_size;
	_pos = (p_pos - _size / 2.0f).ceiling();
	for (int i = -1; i < _size.x + 1; i++)
	{
		for (int j = -1; j < _size.y + 1; j++)
		{
			jgl::Vector2 tmp = jgl::Vector2(i, j);
			Node* node = board_node(tmp);
			_content[tmp] = new Battle_node(tmp);
			if (node == nullptr)
			{
				_content[tmp]->type_background = Battle_node_type::inexistant;
				_content[tmp]->type = Battle_node_type::clear;
			}
			else if (is_middle(0, tmp, _size - 1) == true)
			{
				_content[tmp]->type_background = Battle_node_type::obstacle;
				_content[tmp]->type = Battle_node_type::clear;
			}
			else
			{
				_content[tmp]->type_background = Battle_node_type::border;
				_content[tmp]->type = Battle_node_type::clear;
			}
		}
	}
	_accessible_node = parse_area(_size / 2.0f - 1);
	glGenBuffers(1, &_vertex_buffer);
	glGenBuffers(1, &_uvs_buffer);
	glGenBuffers(1, &_uvs_background_buffer);
	_points.clear();
}

void Battle_arena::define_node_type(jgl::Vector2 pos, Battle_node_type type, bool state)
{
	Battle_node* node = _content[pos];
	node->calculated = state;
	node->type = type;
}

void Battle_arena::define_background_node_type(jgl::Vector2 pos, Battle_node_type type, bool state)
{
	Battle_node* node = _content[pos];
	node->calculated = state;
	node->type_background = type;
}

Node* Battle_arena::board_node(jgl::Vector2 tmp)
{
	return (engine->board()->node(tmp + _pos));
}

Battle_node* Battle_arena::battle_node(jgl::Vector2 tmp)
{
	if (_content.count(tmp) == 0)
		return (nullptr);
	return (_content[tmp]);
}

Battle_node* Battle_arena::absolute_battle_node(jgl::Vector2 tmp)
{
	if (_content.count(tmp - _pos) == 0)
		return (nullptr);
	return (_content[tmp - _pos]);
}

void Battle_arena::reset()
{
	for (size_t i = 0; i < _ally_start_pos.size(); i++)
		define_node_type(_ally_start_pos[i], Battle_node_type::clear, false);
	for (size_t i = 0; i < _enemy_start_pos.size(); i++)
		define_node_type(_enemy_start_pos[i], Battle_node_type::clear, false);
}

void Battle_arena::generate_random_start()
{
	jgl::Array<jgl::Vector2> tmp_accessible_node = jgl::Array<jgl::Vector2>(accessible_node());
	
	for (size_t i = 0; i < 10; i++)
	{
		size_t index = jgl::generate_nbr(0, tmp_accessible_node.size());
		_ally_start_pos.push_back(tmp_accessible_node[index]);
		define_node_type(tmp_accessible_node[index], Battle_node_type::ally_pos, false);
		tmp_accessible_node.erase(index);
	}
	for (size_t i = 0; i < 10; i++)
	{
		size_t index = jgl::generate_nbr(0, tmp_accessible_node.size());
		_enemy_start_pos.push_back(tmp_accessible_node[index]);
		define_node_type(tmp_accessible_node[index], Battle_node_type::enemy_pos, false);
		tmp_accessible_node.erase(index);
	}
	std::cout << tmp_accessible_node.size() << " vs " << accessible_node().size() << std::endl;
}

jgl::Array<jgl::Vector2> Battle_arena::parse_area(jgl::Vector2 start)
{
	static jgl::Vector2 neightbour[4] = {
		   jgl::Vector2(0, 1),
		   jgl::Vector2(1, 0),
		   jgl::Vector2(0, -1),
		   jgl::Vector2(-1, 0),
	};

	jgl::Array<jgl::Vector2> result;
	jgl::Array<jgl::Vector2> to_calc;

	to_calc.push_back(start.round());
	result.push_back(start.round());
	define_background_node_type(start.round(), Battle_node_type::clear, true);
	define_node_type(start.round(), Battle_node_type::clear, true);
	for (size_t i = 0; i < to_calc.size(); i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			jgl::Vector2 tmp = to_calc[i] + neightbour[j];
			Node* tmp_node = board_node(tmp);
			if (_content.count(tmp) != 0)
			{
				Battle_node* tmp_bnode = _content[tmp];
				if (tmp_bnode->calculated == false && is_middle(0, tmp, _size - 1) == true)
				{
					if (tmp_node == nullptr || tmp_node->tile() == nullptr)
					{
						define_node_type(tmp, Battle_node_type::inexistant, true);
						define_background_node_type(tmp, Battle_node_type::inexistant, true);
					}
					else if ((tmp_node->tile()->type & SWIMABLE) == SWIMABLE)
					{
						define_node_type(tmp, Battle_node_type::swimable, true);
						define_background_node_type(tmp, Battle_node_type::swimable, true);
						to_calc.push_back(tmp);
					}
					else if ((tmp_node->tile()->type & WALKABLE) == OBSTACLE || tmp_node->link() != nullptr)
					{
						define_node_type(tmp, Battle_node_type::obstacle, true);
						define_background_node_type(tmp, Battle_node_type::obstacle, true);
					}
					else if (tmp_node->link() == nullptr && tmp_bnode->calculated == false)
					{
						result.push_back(tmp);
						define_node_type(tmp, Battle_node_type::clear, true);
						define_background_node_type(tmp, Battle_node_type::clear, true);
						to_calc.push_back(tmp);
					}
				}
			}
		}
	}

	return (result);
}

void Battle_arena::rebake()
{
	_points.clear();
	bake_background();
	bake();
}

void Battle_arena::place(Creature_entity* entity, jgl::Vector2 pos)
{
	_content[pos]->occupant = entity;
	entity->place(_pos + pos);
}

void Battle_arena::move(Creature_entity* entity, jgl::Vector2 delta)
{
	_content[entity->pos()]->occupant = nullptr;
	_content[entity->pos() + delta]->occupant = entity;
	entity->move(delta, false);
}

void Battle_arena::bake_background()
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
	_uvs_background.clear();
	jgl::Vector2 unit = engine->battle_tileset()->unit();
	for (auto it : _content)
	{
		if (it.second != nullptr)
		{
			if (it.second->type_background != Battle_node_type::inexistant)
			{
				jgl::Vector2 tmp_sprite = engine->battle_tileset()->sprite(jgl::Vector2(static_cast<int>(it.second->type_background), 0));
				for (size_t h = 0; h < 6; h++)
				{
					jgl::Vector2 tmp_value = tmp_sprite + unit * neightbour[h];
					_uvs_background.push_back(tmp_value);
				}
			}
			else
			{
				for (size_t h = 0; h < 6; h++)
					_uvs_background.push_back(-1);
			}
		}
	}
	const jgl::Vector2* tmp_uvs = _uvs_background.all();

	glBindBuffer(GL_ARRAY_BUFFER, _uvs_background_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _uvs_background.size() * 2, static_cast<const float*>(&(tmp_uvs[0].x)), GL_STATIC_DRAW);

}

void Battle_arena::bake()
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
			if (it.second->type != Battle_node_type::inexistant && it.second->type != Battle_node_type::border &&
				it.second->type != Battle_node_type::clear && it.second->type != Battle_node_type::obstacle &&
				it.second->type != Battle_node_type::swimable)
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _uvs.size() * 2, static_cast<const float*>(&(tmp_uvs[0].x)), GL_STATIC_DRAW);

}

void Battle_arena::render(jgl::Viewport* p_viewport, jgl::Vector2 base_pos)
{
	render_background(p_viewport, base_pos);
	render_front(p_viewport, base_pos);
}

void Battle_arena::render_background(jgl::Viewport* p_viewport, jgl::Vector2 base_pos)
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
	glBindBuffer(GL_ARRAY_BUFFER, _uvs_background_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_points.size()));
}

void Battle_arena::render_front(jgl::Viewport* p_viewport, jgl::Vector2 base_pos)
{
	if (p_viewport != nullptr)
		p_viewport->use();

	jgl::Vector2 delta = jgl::convert_screenV2_to_opengl(tile_to_screen(_pos, base_pos));

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

bool Battle_arena::can_acces(jgl::Vector2 pos, jgl::Vector2 delta)
{
	jgl::Vector2 norm_delta = delta.normalize().round();
	static jgl::Vector2 move_delta[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };

	size_t i = 0;
	for (; i < 4; i++)
		if (norm_delta == move_delta[i])
			break;
	if (i == 4)
		return (false);
	Battle_node* tmp = battle_node((pos + delta).round());
	Battle_node* actual = battle_node(pos.round());
	if (tmp != nullptr && actual != nullptr &&
		tmp->type  == Battle_node_type::clear &&
		actual->type == Battle_node_type::clear)
	{
		return (true);
	}
	return (false);
}

Battle_node* Battle_arena::find_closest(jgl::Array<jgl::Vector2>& to_calc)
{
	Battle_node* result = nullptr;

	for (size_t i = 0; i < to_calc.size(); i++)
	{
		Battle_node* other = battle_node(to_calc[i]);
		if (other != nullptr && other->calculated == false && (result == nullptr || (result->t_cost > other->t_cost)))
			result = other;
	}
	return (result);
}

jgl::Array<jgl::Vector2> Battle_arena::pathfinding(jgl::Vector2 start, jgl::Vector2 end)
{
	start = start - _pos;
	end = end - _pos;

	if (start == end ||
		battle_node(start) == nullptr || battle_node(start)->type != Battle_node_type::clear ||
		battle_node(end) == nullptr || battle_node(end)->type != Battle_node_type::clear)
		return (jgl::Array<jgl::Vector2>());

	for (auto it : _content)
		it.second->calculated = false;

	static jgl::Vector2 neightbour[4] = { jgl::Vector2(0.0f, 1.0f), jgl::Vector2(0.0f, -1.0f), jgl::Vector2(-1.0f, 0.0f), jgl::Vector2(1.0f, 0.0f) };

	jgl::Array<jgl::Vector2> result;
	jgl::Array<jgl::Vector2> to_calc;

	battle_node(start)->calc_cost(0, end);
	if (battle_node(start)->t_cost >= 30)
		return (jgl::Array<jgl::Vector2>());
	to_calc.push_back(start);
	Battle_node* target_node = battle_node(start);
	target_node->parent = nullptr;
	while (target_node != nullptr)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (can_acces(target_node->pos, neightbour[j]) == true)
			{
				jgl::Vector2 actual = target_node->pos + neightbour[j];
				Battle_node* actual_node = battle_node(actual);
				if (actual_node != nullptr && actual_node->calculated == false)
				{
					actual_node->calc_cost(target_node->s_cost + 1, end);
					if (actual_node->t_cost > 30)
						return (jgl::Array<jgl::Vector2>());
					battle_node(actual)->parent = target_node;
					to_calc.push_back(actual);
				}
			}
		}
		target_node->calculated = true;
		target_node = find_closest(to_calc);

		if (target_node != nullptr && target_node->pos == end)
			target_node = nullptr;
	}
	for (size_t i = 0; i < to_calc.size(); i++)
		battle_node(to_calc[i])->calculated = false;
	target_node = battle_node(end);
	while (target_node != nullptr)
	{
		result.push_back(target_node->pos);
		target_node = target_node->parent;
	}
	result.reverse();
	return (result);
}