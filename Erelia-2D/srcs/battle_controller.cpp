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
	},
	{
		"",
		"",
		"",
		""
	}
};

static void start_action_mode(jgl::Data data)
{
	std::cout << "Action mode !" << std::endl;
	engine->battle_mode()->arena()->reset();
	engine->battle_mode()->controller()->change_phase(Battle_action_phase::attack);
}

static void start_move_mode(jgl::Data data)
{
	std::cout << "Movement mode !" << std::endl;
	engine->battle_mode()->check_movement();
	engine->battle_mode()->controller()->change_phase(Battle_action_phase::base);
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

static void cast_spell_action(jgl::Data data)
{
	int value = (int)(data.acces<void*>(1));
	std::cout << "Casting spell number : " << value << " !" << std::endl;
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
	},
	{
		cast_spell_action,
		cast_spell_action,
		cast_spell_action,
		cast_spell_action
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
	},
	{
		jgl::Data(2, nullptr, (void *)0),
		jgl::Data(2, nullptr, (void*)1),
		jgl::Data(2, nullptr, (void*)2),
		jgl::Data(2, nullptr, (void*)3)
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

	jgl::Widget* tmp_panel = _panels[static_cast<size_t>(Battle_action_phase::attack)];

	_close_attack_button = new jgl::Button(button_funct[static_cast<size_t>(Battle_action_phase::base)][1], button_data[static_cast<size_t>(Battle_action_phase::base)][1], tmp_panel);
	_close_attack_button->activate();
	_close_attack_button->set_text("X");
	_close_attack_button->send_front();

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
	_close_attack_button->set_geometry(jgl::Vector2(p_area.x - 30, 0.0f), 30);
}

void Battle_controller::render()
{

}
