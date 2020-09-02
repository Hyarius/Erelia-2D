#include "erelia.h"

jgl::Array<jgl::Vector3> points;
GLuint vertex_buffer = 0;

bool Chunk::is_border(int x, int y)
{
	if (x == 0 || y == 0 || x == CHUNK_SIZE - 1 || y == CHUNK_SIZE - 1)
		return (true);
	return (false);
}

Node* Chunk::node(jgl::Vector2 pos)
{
	int x = static_cast<int>(pos.x);
	int y = static_cast<int>(pos.y);
	if (x < 0 || x >= CHUNK_SIZE ||
		y < 0 || y >= CHUNK_SIZE)
		return (nullptr);
	return (_content[x][y]);
}

Chunk::Chunk(jgl::Vector2 p_pos)
{
	_pos = p_pos;
	glGenBuffers(1, &_uvs_buffer);
	glGenBuffers(1, &_uvs_buffer2);

	for (size_t i = 0; i < CHUNK_SIZE; i++)
		for (size_t j = 0; j < CHUNK_SIZE; j++)
			_content[i][j] = new Node(nullptr, jgl::Vector2(i, j) + _pos * CHUNK_SIZE);
}

void Chunk::place(jgl::Vector2 coord, Tile* p_tile, bool need_bake)
{
	int x = jgl::floor(coord.x);
	int y = jgl::floor(coord.y);
	_content[x][y]->set_tile(p_tile);
}

void Chunk::bake()
{
	static jgl::Vector2 neightbour[6] = {
		jgl::Vector2(0, 0),
		jgl::Vector2(0, 1),
		jgl::Vector2(1, 0),
		jgl::Vector2(0, 1),
		jgl::Vector2(1, 1),
		jgl::Vector2(1, 0),
	};
	_uvs.clear();
	_uvs2.clear();
	if (points.size() == 0)
	{
		jgl::Vector2 vtmp = (jgl::Vector2(node_size) / (g_application->size() / 2)) * jgl::Vector2(1, -1);
		std::cout << std::endl << "------" << std::endl << std::endl;
		std::cout << "Ratio over " << g_application->size() << " = " << jgl::Vector2(node_size) / g_application->size() << std::endl;
 		if (vertex_buffer != 0)
			glDeleteBuffers(1, &vertex_buffer);
		glGenBuffers(1, &vertex_buffer);

		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int j = 0; j < CHUNK_SIZE; j++)
			{
				for (size_t h = 0; h < 6; h++)
				{
					jgl::Vector2 value = (jgl::Vector2(i, j) + neightbour[h]) * vtmp;
					points.push_back(jgl::Vector3(value.x, value.y, 0.0f));
				}
			}

		const jgl::Vector3 *tmp = points.all();

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size() * 3, static_cast<const float*>(&(tmp[0].x)), GL_STATIC_DRAW);
	}
	for (int i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_SIZE; j++)
			if (_content[i][j]->tile() != nullptr)
			{
				jgl::Vector2 unit = engine->tileset()->unit();
				jgl::Vector2 tmp_sprite = engine->tileset()->sprite(_content[i][j]->tile()->sprite);

				for (size_t h = 0; h < 6; h++)
					_uvs.push_back(tmp_sprite + unit * neightbour[h]);
			}
			else
			{
				for (size_t h = 0; h < 6; h++)
					_uvs.push_back(-1);
			}
	for (int i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_SIZE; j++)
			if (_content[i][j]->tile() != nullptr)
			{
				jgl::Vector2 unit = engine->tileset()->unit();
				jgl::Vector2 tmp_sprite = engine->tileset()->sprite(_content[i][j]->tile()->sprite + jgl::Vector2(0, 72));

				for (size_t h = 0; h < 6; h++)
					_uvs2.push_back(tmp_sprite + unit * neightbour[h]);
			}
			else
			{
				for (size_t h = 0; h < 6; h++)
					_uvs2.push_back(-1);
			}

	const jgl::Vector2* tmp_uvs = _uvs.all();

	glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* points.size() * 2, static_cast<const float*>(&(tmp_uvs[0].x)), GL_STATIC_DRAW);

	const jgl::Vector2* tmp_uvs2 = _uvs2.all();

	glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size() * 2, static_cast<const float*>(&(tmp_uvs2[0].x)), GL_STATIC_DRAW);
}

void Chunk::update()
{
	for (size_t i = 0; i < CHUNK_SIZE; i++)
		for (size_t j = 0; j < CHUNK_SIZE; j++)
			if (_content[i][j]->occupant() != nullptr && _content[i][j]->occupant()->type() != Entity_type::Player)
				_content[i][j]->occupant()->update();
}

void Chunk::render(jgl::Viewport* viewport)
{
	jgl::Vector2 vtmp = (jgl::Vector2(node_size) / (g_application->size() / 2)) * jgl::Vector2(1, -1);
	jgl::Vector2 delta = vtmp.invert() / 2 + vtmp * (_pos * CHUNK_SIZE - engine->player()->pos());

	glBindVertexArray(g_application->vertex_array());

	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, engine->tileset()->image()->texture_id());

	glUniform3f(delta_pos_uniform, delta.x, delta.y, 0.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, (jgl::get_frame_state(2) == 0 ? _uvs_buffer : _uvs_buffer2));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(points.size()));
}

void Chunk::render_entity(jgl::Viewport* viewport)
{
	for (size_t i = 0; i < CHUNK_SIZE; i++)
		for (size_t j = 0; j < CHUNK_SIZE; j++)
			if (_content[i][j]->occupant() != nullptr && _content[i][j]->occupant()->type() != Entity_type::Player)
				_content[i][j]->occupant()->render(viewport);
}