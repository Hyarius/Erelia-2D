#ifndef ERELIA_BATTLER_H
#define ERELIA_BATTLER_H

struct Encounter_data
{
	static Encounter_data null() { return (Encounter_data(-1, -1)); }

	int id;
	int probability;
	Encounter_data(int p_id = -1, int p_probability = -1)
	{
		id = p_id;
		probability = p_probability;
	}

	bool operator == (const Encounter_data delta) const
	{
		if (id == null().id)
			return (true);
		return (false);
	}
	bool operator != (const Encounter_data delta) const
	{
		if (id == null().id)
			return (false);
		return (true);
	}
};
enum class Battle_node_type
{
	inexistant = -1,
	clear = 0,
	obstacle = 1,
	select = 2,
	ally = 3,
	enemy = 4,
	mouvement = 5,
	range = 6,
	ally_pos = 7,
	enemy_pos = 8
};

struct Battle_node
{
	jgl::Vector2 pos;
	Battle_node_type type;

	bool calculated;
	size_t s_cost;
	size_t e_cost;
	size_t t_cost;
	Node* parent;

	Battle_node(jgl::Vector2 p_pos)
	{
		pos = p_pos;
		type = Battle_node_type::clear;
		calculated = false;
		parent = nullptr;
	}
};

class Battle_area
{
private:
	jgl::Vector2 _pos;
	jgl::Vector2 _size;
	Battle_node*** _content;

	GLuint _vertex_buffer;
	jgl::Array<jgl::Vector3> _points;
	jgl::Array<jgl::Vector2> _uvs;
	GLuint _uvs_buffer;
public:
	jgl::Vector2 pos() { return (_pos); }
	jgl::Vector2 size() { return (_size); }

	Battle_area(jgl::Vector2 p_pos, jgl::Vector2 p_size);
	void rebake();
	void bake();
	void render(jgl::Viewport* p_viewport, jgl::Vector2 base_pos);
};

class Battle_data
{
private:
	jgl::Color _color;
	jgl::Vector2 _min;
	jgl::Vector2 _max;
	jgl::Array<jgl::Vector2> _tile_array;
	jgl::Array<Encounter_data> _encounter_array;
	int _probability;

public:
	Battle_data();
	void set_color(jgl::Color p_color) { _color = p_color; }
	void set_probability(int p_probability) { _probability = p_probability; }
	int probability() { return (_probability); }
	jgl::Color color() { return (_color); }
	jgl::Array<jgl::Vector2>& tile_array() { return (_tile_array); }
	jgl::Array<Encounter_data>& encounter_array() { return (_encounter_array); }
	void add_tile(jgl::Vector2 pos);
	void add_encouter(Encounter_data data);
	bool is_inside(jgl::Vector2 pos);
	Encounter_data ask();
};

#endif