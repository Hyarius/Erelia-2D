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

	std::map<std::string, jgl::Vector2> _warps;

	jgl::Array<Link*> _links;
	jgl::Array<jgl::Vector3> _points;
	GLuint _vertex_buffer;

public:
	jgl::Sprite_sheet* tileset() { return (_tileset); }
	jgl::Sprite_sheet* charset() { return (_charset); }
	Chunk* chunk(jgl::Vector2 pos);
	Tile* tile(jgl::Vector2 pos);
	Node* node(jgl::Vector2 pos);
	Board(jgl::Sprite_sheet* p_tileset, jgl::Sprite_sheet* p_charset, jgl::String path);
	~Board();
	void clear();
	void save(jgl::String path);
	void load_warp(std::fstream& file, jgl::Array<jgl::String> tab);
	void load_chunk(std::fstream& file, jgl::Array<jgl::String> tab);
	void load_npc(std::fstream& file, jgl::Array<jgl::String> tab);
	void load_link(std::fstream& file, jgl::Array<jgl::String> tab);
	void load(jgl::String path);
	std::map<std::string, jgl::Vector2>& warps() { return (_warps); }
	jgl::Vector2 warp(jgl::String name) { if (_warps.contains(name.std()) == 0)return (-1); return (_warps[name.std()]); }
	void add_warp(jgl::String name, jgl::Vector2 pos);
	void add_link(jgl::Vector2 p_a, jgl::Vector2 p_b, bool dual);
	void add_npc(NPC* p_npc);
	void remove_link(jgl::Vector2 pos);
	void remove_warp(jgl::String name);
	void remove_npc(jgl::String name);
	jgl::Vector2 chunk_pos(jgl::Vector2 node_pos);
	jgl::Vector2 rel_node_pos(jgl::Vector2 node_pos);
	void place(jgl::Vector2 node_pos, size_t index, bool need_bake = false);
	void place(jgl::Vector2 node_pos, Tile* p_node, bool need_bake = false);
	void bake_chunk(jgl::Vector2 pos);
	void update(jgl::Vector2 player_pos);
	void render(jgl::Vector2 player_pos, jgl::Viewport* viewport);

	bool can_acces(jgl::Vector2 pos, jgl::Vector2 delta);
	Node* find_closest(jgl::Array<jgl::Vector2>& to_calc);
	jgl::Array<jgl::Vector2> pathfinding(jgl::Vector2 start, jgl::Vector2 end);
};

#endif