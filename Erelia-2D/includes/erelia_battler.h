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

class Battle_area
{
private:
	jgl::Vector2 _pos;
	static jgl::Vector2 _size;
public:
	Battle_area(jgl::Vector2 p_pos, jgl::Vector2 p_size);

	void render();
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