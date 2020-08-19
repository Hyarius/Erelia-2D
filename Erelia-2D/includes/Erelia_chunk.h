#ifndef ERELIA_CHUNK_H
#define ERELIA_CHUNK_H

#include "jgl.h"
#include "erelia_node.h"

class Chunk
{
private:
	jgl::Vector2 _pos;
	Node* _content[CHUNK_SIZE][CHUNK_SIZE];

	jgl::Array<jgl::Vector2> _uvs;

	GLuint _uvs_buffer;

public:
	bool is_border(int x, int y);
	Node* node(jgl::Vector2 pos);
	Chunk(jgl::Vector2 p_pos);
	~Chunk()
	{
		for (size_t i = 0; i < CHUNK_SIZE; i++)
			for (size_t j = 0; j < CHUNK_SIZE; j++)
				delete _content[i][j];
	}
	void place(jgl::Vector2 coord, Tile* p_tile, bool need_bake = false);
	void bake();
	void update();
	void render(jgl::Viewport* viewport);
	void render_entity(jgl::Viewport* viewport);
};

#endif