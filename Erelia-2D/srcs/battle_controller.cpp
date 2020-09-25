#include "erelia.h"

jgl::Array< jgl::Array<jgl::String> > button_text = {
	{

	},
	{

	},
	{
	"Attack",
	"Move",
	"Change",
	"Flee"
	}
};

static void start_action_mode(jgl::Data data)
{
	std::cout << "Action mode !" << std::endl;
}

static void start_move_mode(jgl::Data data)
{
	std::cout << "Movement mode !" << std::endl;
	engine->battle_mode()->check_movement();
}

static void start_change_mode(jgl::Data data)
{
	std::cout << "Change mode !" << std::endl;
}

static void start_flee_mode(jgl::Data data)
{
	std::cout << "Flee mode !" << std::endl;
	engine->battle_mode()->change_phase();
}

jgl::Array< jgl::Array<jgl::Funct> > button_funct = {
	{

	},
	{

	},
	{
		start_action_mode,
		start_move_mode,
		start_change_mode,
		start_flee_mode
	}
};

jgl::Array< jgl::Array<jgl::Data> > button_data = {
	{

	},
	{

	},
	{
		jgl::Data(nullptr),
		jgl::Data(nullptr),
		jgl::Data(nullptr),
		jgl::Data(nullptr)
	}
};

Battle_controller::Battle_controller(jgl::Widget* parent) : jgl::Widget(parent)
{
	_frame = new jgl::Frame(this);
	_frame->activate();
	size_t nb_phase = static_cast<size_t>(Battle_action_phase::count);
	_panels.resize(nb_phase);
	for (size_t nb = 0; nb < nb_phase; nb++)
	{
		if (nb != 0 && nb != 1)
		{
			_panels[nb] = new Battle_controller_panel(button_text[nb], button_funct[nb], button_data[nb], _frame);
		}
		else
		{
			_panels[nb] = nullptr;
		}
	}	
	change_phase(Battle_action_phase::placement);
}

void Battle_controller::change_phase(Battle_action_phase phase)
{
	_phase = phase;
	size_t nb_phase = static_cast<size_t>(_phase);
	for (size_t i = 0; i < _panels.size(); i++)
		if (_panels[i] != nullptr)
			_panels[i]->desactivate();
	if (nb_phase < _panels.size() && _panels[nb_phase] != nullptr)
	{
		std::cout << "Here ?" << std::endl;
		_panels[nb_phase]->activate();
	}
}

void Battle_controller::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_frame->set_geometry(0, p_area);
	for (size_t i = 0; i < _panels.size(); i++)
		if (_panels[i] != nullptr)
			_panels[i]->set_geometry(0, p_area);
}

void Battle_controller::render()
{

}
