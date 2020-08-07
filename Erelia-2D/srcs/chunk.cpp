#include "erelia.h"

bool Chunk::is_border(int x, int y)
{
	if (x == 0 || y == 0 || x == chunk_size - 1 || y == chunk_size - 1)
		return (true);
	return (false);
}

Node* Chunk::node(jgl::Vector2 pos)
{
	int x = static_cast<int>(pos.x);
	int y = static_cast<int>(pos.y);
	if (x < 0 || x >= chunk_size ||
		y < 0 || y >= chunk_size)
		return (nullptr);
	return (_content[x][y]);
}

Chunk::Chunk(jgl::Vector2 p_pos)
{
	_pos = p_pos;
	glGenBuffers(1, &_uvs_buffer);
	glGenBuffers(1, &_vertex_buffer);

	for (size_t i = 0; i < chunk_size; i++)
		for (size_t j = 0; j < chunk_size; j++)
			_content[i][j] = new Node(nullptr);
}

void Chunk::place(jgl::Vector2 coord, Tile* p_tile, bool need_bake)
{
	int x = jgl::floor(coord.x);
	int y = jgl::floor(coord.y);
	_content[x][y]->set_tile(p_tile);
}

void Chunk::bake(jgl::Sprite_sheet* tileset)
{
	_points.clear();
	_uvs.clear();
	for (int i = 0; i < chunk_size; i++)
		for (int j = 0; j < chunk_size; j++)
			if (_content[i][j]->tile() != nullptr)
			{
				jgl::Vector2 tmp = (jgl::Vector2(i, j) + _pos * chunk_size) * node_size;

				_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(0, 0) * node_size));
				_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(0, 1) * node_size));
				_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(1, 0) * node_size));
				_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(0, 1) * node_size));
				_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(1, 1) * node_size));
				_points.push_back(jgl::convert_screen_to_opengl(tmp + jgl::Vector2(1, 0) * node_size));

				jgl::Vector2 tmp_sprite = tileset->sprite(_content[i][j]->tile()->sprite);

				_uvs.push_back(tmp_sprite + jgl::Vector2(0.0f, 0.0f));
				_uvs.push_back(tmp_sprite + jgl::Vector2(0.0f, tileset->unit().y));
				_uvs.push_back(tmp_sprite + jgl::Vector2(tileset->unit().x, 0.0f));
				_uvs.push_back(tmp_sprite + jgl::Vector2(0.0f, tileset->unit().y));
				_uvs.push_back(tmp_sprite + tileset->unit());
				_uvs.push_back(tmp_sprite + jgl::Vector2(tileset->unit().x, 0.0f));
			}

	const jgl::Vector3* tmp = _points.all();
	const jgl::Vector2* tmp2 = _uvs.all();

	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _points.size() * 3, static_cast<const float*>(&(tmp[0].x)), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _points.size() * 2, static_cast<const float*>(&(tmp2[0].x)), GL_STATIC_DRAW);
}

void Chunk::render(jgl::Sprite_sheet* tileset, jgl::Vector2 center, jgl::Viewport* viewport)
{
	glBindVertexArray(g_application->vertex_array());

	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tileset->image()->texture_id());

	glUniform3f(delta_pos_uniform, center.x, center.y, 0.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _uvs_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_points.size()));

}