#ifndef ERELIA_BATTLE_CONTROLLER_H
#define ERELIA_BATTLE_CONTROLLER_H

enum class Battle_action_phase
{
	placement = 0,
	enemy_turn = 1,
	base = 2,
	count
};

class Battle_controller_panel : public jgl::Widget
{
private:
	jgl::Button* _buttons[4];

public:
	Battle_controller_panel(const jgl::Array<jgl::String>& button_text, const jgl::Array<jgl::Funct>& functs, const jgl::Array<jgl::Data>& datas, jgl::Widget* parent = nullptr) : jgl::Widget(parent)
	{
		for (size_t i = 0; i < 4; i++)
		{
			_buttons[i] = new jgl::Button(functs[i], datas[i], this);
			_buttons[i]->set_text(button_text[i]);
			_buttons[i]->activate();
		}
	}
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		jgl::Vector2 size = p_area / 2;
		for (size_t i = 0; i < 4; i++)
		{
			_buttons[i]->set_geometry(size * jgl::Vector2(i % 2, i / 2) + 5.0f, size - 10);
			_buttons[i]->set_size(30);
		}
	}
	void render()
	{

	}
};

class Battle_controller : public jgl::Widget
{
private:
	Battle_action_phase _phase;

	jgl::Frame* _frame;
	jgl::Array<Battle_controller_panel*> _panels;

public:
	Battle_controller(jgl::Widget* parent = nullptr);

	void change_phase(Battle_action_phase phase);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif