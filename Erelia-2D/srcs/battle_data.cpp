#include "erelia.h"

Battle_data::Battle_data()
{
	_min = 0;
	_max = 0;
	_probability = 0;
	_color = jgl::Color(0, 0, 0, 0);
}

void Battle_data::add_tile(jgl::Vector2 pos)
{
	_tile_array.push_back(pos);
	_min = jgl::compose_smaller(pos, _min);
	_max = jgl::compose_biggest(pos, _max);
}

void Battle_data::add_encouter(Encounter_data data)
{
	_encounter_array.push_back(data);
}

bool Battle_data::is_inside(jgl::Vector2 pos)
{
	if (jgl::is_middle(_min, pos, _max) == true)
	{
		for (size_t i = 0; i < _tile_array.size(); i++)
			if (pos == _tile_array[i])
				return (true);
	}
	return (false);
}

Encounter_data Battle_data::ask()
{
	int value = jgl::generate_nbr(0, 100);

	std::cout << "Probability : " << _probability << " vs " << value << std::endl;
	if (value < _probability)
	{
		value = jgl::generate_nbr(0, 100);
		for (size_t i = 0; i < _encounter_array.size(); i++)
		{
			if (value < _encounter_array[i].probability)
				return (_encounter_array[i]);
			value -= _encounter_array[i].probability;
		}
	}
	return (Encounter_data::null());
}