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
	border = 2,
	swimable = 3,
	cursor = 4,
	select = 5,
	ally = 6,
	enemy = 7,
	mouvement = 8,
	range = 9,
	ally_pos = 10,
	enemy_pos = 11
};

struct Battle_node
{
	jgl::Vector2 pos;
	Battle_node_type type_background;
	Battle_node_type type;
	class Creature_entity* occupant;

	bool calculated;
	size_t s_cost;
	size_t e_cost;
	size_t t_cost;
	Battle_node* parent;

	Battle_node(jgl::Vector2 p_pos)
	{
		pos = p_pos;
		type = Battle_node_type::clear;
		type_background = Battle_node_type::clear;
		calculated = false;
		s_cost = 0;
		e_cost = 0;
		t_cost = 0;
		parent = nullptr;
		occupant = nullptr;
	}
	void calc_cost(size_t start_cost, jgl::Vector2 end)
	{
		size_t e = static_cast<int>(std::abs(pos.x - end.x)) + static_cast<int>(std::abs(pos.y - end.y));

		s_cost = start_cost;
		e_cost = e;
		t_cost = start_cost + e;
	}
};

class Battle_arena
{
private:
	jgl::Vector2 _pos;
	jgl::Vector2 _size;
	std::map<jgl::Vector2, Battle_node*> _content;
	jgl::Array<jgl::Vector2> _accessible_node;

	GLuint _vertex_buffer;
	jgl::Array<jgl::Vector3> _points;

	GLuint _uvs_buffer;
	jgl::Array<jgl::Vector2> _uvs;

	GLuint _uvs_background_buffer;
	jgl::Array<jgl::Vector2> _uvs_background;
public:
	jgl::Vector2 pos() { return (_pos); }
	jgl::Vector2 size() { return (_size); }
	jgl::Array<jgl::Vector2> accessible_node() { return (_accessible_node); }

	void define_node_type(jgl::Vector2 pos, Battle_node_type type, bool state);
	void define_background_node_type(jgl::Vector2 pos, Battle_node_type type, bool state);
	Node* board_node(jgl::Vector2 tmp);
	Battle_node* battle_node(jgl::Vector2 tmp);
	Battle_node* absolute_battle_node(jgl::Vector2 tmp);

	std::map<jgl::Vector2, Battle_node*>& content() {return (_content);}

	Battle_arena(jgl::Vector2 p_pos, jgl::Vector2 p_size);

	void place(Creature_entity* entity, jgl::Vector2 pos);
	void move(Creature_entity* entity, jgl::Vector2 delta);

	jgl::Array<jgl::Vector2> parse_area(jgl::Vector2 start);
	void rebake();
	void bake_background();
	void bake();
	void render_background(jgl::Viewport* p_viewport, jgl::Vector2 base_pos);
	void render_front(jgl::Viewport* p_viewport, jgl::Vector2 base_pos);
	void render(jgl::Viewport* p_viewport, jgl::Vector2 base_pos);

	bool can_acces(jgl::Vector2 pos, jgl::Vector2 delta);
	Battle_node* find_closest(jgl::Array<jgl::Vector2>& to_calc);
	jgl::Array<jgl::Vector2> pathfinding(jgl::Vector2 start, jgl::Vector2 end);
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