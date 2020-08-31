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
	jgl::Color _color;
	jgl::Vector2 _min;
	jgl::Vector2 _max;
	jgl::Array<jgl::Vector2> _tile_array;
	jgl::Array<Encounter_data> _encounter_array;

public:
	Battle_area()
	{
		_min = 0;
		_max = 0;
		_color = jgl::Color(0, 0, 0, 0);
	}
	void set_color(jgl::Color p_color)
	{
		_color = p_color;
	}
	jgl::Color color() { return (_color); }
	jgl::Array<jgl::Vector2> tile_array() { return (_tile_array); }
	jgl::Array<Encounter_data> encounter_array() { return (_encounter_array); }
	void add_tile(jgl::Vector2 pos)
	{
		_tile_array.push_back(pos);
		_min = jgl::compose_smaller(pos, _min);
		_max = jgl::compose_biggest(pos, _max);
	}
	void add_encouter(Encounter_data data)
	{
		_encounter_array.push_back(data);
	}
	bool is_inside(jgl::Vector2 pos)
	{
		if (jgl::is_middle(_min, pos, _max) == false)
		{
			return (false);
		}
		else
		{
			return (true);
		}
	}
	Encounter_data ask()
	{
		int value = jgl::generate_nbr(0, 100);

		for (size_t i = 0; i < _encounter_array.size(); i++)
		{
			if (_encounter_array[i].probability <= value)
				return (_encounter_array[i]);
			value -= _encounter_array[i].probability;
		}
		return (Encounter_data::null());
	}
};

#endif