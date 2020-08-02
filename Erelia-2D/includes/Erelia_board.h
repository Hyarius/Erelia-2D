#ifndef ERELIA_BOARD_H
#define ERELIA_BOARD_H

#include "erelia_chunk.h"
#include "erelia_npc.h"

class Board
{
private:
	std::map<jgl::Vector2, Chunk*> _chunks;

	jgl::Sprite_sheet* _tileset = nullptr;
	jgl::Sprite_sheet* _charset = nullptr;

	jgl::Array<NPC*> _npc_array;

	jgl::Array<jgl::Vector3> _points;
	GLuint _vertex_buffer;

public:
	jgl::Sprite_sheet* tileset() { return (_tileset); }
	Chunk* chunk(jgl::Vector2 pos);
	Node* node(jgl::Vector2 pos);
	Board(jgl::Sprite_sheet* p_tileset, jgl::Sprite_sheet* p_charset, jgl::String path);
	~Board();
	void save(jgl::String path);
	void load_chunk(std::fstream& file, jgl::Array<jgl::String> tab);
	void load_npc(std::fstream& file, jgl::Array<jgl::String> tab);
	void load(jgl::String path);
	jgl::Vector2 chunk_pos(jgl::Vector2 node_pos);
	jgl::Vector2 rel_node_pos(jgl::Vector2 node_pos);
	void place(jgl::Vector2 node_pos, size_t index, bool need_bake = false);
	void place(jgl::Vector2 node_pos, Node* p_node, bool need_bake = false);
	void bake_chunk(jgl::Vector2 pos);
	void render(jgl::Vector2 player_pos, jgl::Viewport* viewport);
};

#endif