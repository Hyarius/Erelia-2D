#include "erelia.h"

NPC::NPC(jgl::String p_name, jgl::Vector2 p_pos, jgl::Sprite_sheet* p_charset, jgl::Vector2 p_sprite) : Entity(Entity_type::NPC, p_name, p_pos, p_charset, p_sprite)
{
	_starting_pos = p_pos;
	_road_index = 0;
	_check_point_index = 1;
	_check_point.push_back(_pos);
	_look_dir = Entity_direction::south;
	set_move_speed(0.4f);
	set_wait_time(150);
}

void NPC::calc_next_road(Board* board)
{
	if (_check_point.size() <= 1)
		_road = jgl::Array<jgl::Vector2>();
	else
	{
		_road = board->pathfinding(_pos.round(), _check_point[_check_point_index].round());
		_check_point_index = (_check_point_index + 1) % _check_point.size();
		_road_index = 1;
	}
}

void NPC::update(Board* board)
{
	if (is_moving() == false)
	{
		if (_road.size() != 0 && _road_index != _road.size())
		{
			jgl::Vector2 tmp = (_road[_road_index] - _pos).round();
			if (can_move(board, tmp) == true)
			{
				move(board, tmp);
				_road_index++;
			}
		}
		else
		{
			calc_next_road(board);
		}
	}
	if (is_moving() == true)
	{
		update_pos(board);
	}
}

void NPC::interact()
{
	std::cout << "Interact with an NPC" << std::endl;
}

void NPC::save(std::fstream& file)
{
	file << _name << ";";
	file << static_cast<int>(_starting_pos.x) << ";" << static_cast<int>(_starting_pos.y) << ";";
	file << static_cast<int>(_sprite.x) << ";" << static_cast<int>(_sprite.y) << ";";
	for (size_t i = 0; i < _check_point.size(); i++)
	{
		if (i != 0)
			file << ";";
		file << static_cast<int>(_check_point[i].x) << ";" << static_cast<int>(_check_point[i].y);
	}
	file << std::endl;
}