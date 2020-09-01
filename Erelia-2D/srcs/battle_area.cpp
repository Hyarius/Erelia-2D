#include "erelia.h"

Battle_area::Battle_area()
{
	_min = 0;
	_max = 0;
	_probability = 0;
	_color = jgl::Color(0, 0, 0, 0);
}

void Battle_area::add_tile(jgl::Vector2 pos)
{
	_tile_array.push_back(pos);
	_min = jgl::compose_smaller(pos, _min);
	_max = jgl::compose_biggest(pos, _max);
}

void Battle_area::add_encouter(Encounter_data data)
{
	_encounter_array.push_back(data);
}

bool Battle_area::is_inside(jgl::Vector2 pos)
{
	if (jgl::is_middle(_min, pos, _max) == true)
	{
		for (size_t i = 0; i < _tile_array.size(); i++)
			if (pos == _tile_array[i])
				return (true);
	}
	return (false);
}

Encounter_data Battle_area::ask()
{
	int value = jgl::generate_nbr(0, 100);

	if (value > _probability)
	{
		value = jgl::generate_nbr(0, 100);
		for (size_t i = 0; i < _encounter_array.size(); i++)
		{
			if (_encounter_array[i].probability <= value)
				return (_encounter_array[i]);
			value -= _encounter_array[i].probability;
		}
	}
	return (Encounter_data::null());
}