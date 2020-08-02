#ifndef ERELIA_CHUNK_H
#define ERELIA_CHUNK_H

#include "jgl.h"
#include "erelia_node.h"

class Chunk
{
private:
	jgl::Vector2 _pos;
	Node* _content[chunk_size][chunk_size];

	jgl::Array<jgl::Vector3> _points;
	jgl::Array<jgl::Vector2> _uvs;

	GLuint _vertex_buffer;
	GLuint _uvs_buffer;

public:
	bool is_border(int x, int y);
	Node* node(jgl::Vector2 pos);
	Chunk(jgl::Vector2 p_pos);
	void place(jgl::Vector2 coord, Node* p_node, bool need_bake = false);
	void bake(jgl::Sprite_sheet* tileset);
	void render(jgl::Sprite_sheet* tileset, jgl::Vector2 center, jgl::Viewport* viewport);
};

#endif