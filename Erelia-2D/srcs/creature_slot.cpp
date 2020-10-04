#include "erelia.h"

Creature_slot::Creature_slot(Creature_entity** p_creature, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_selected = false;
	_creature = p_creature;
	_hp_bar = new jgl::Progressive_bar(0.5f, jgl::Color(255, 0, 0), this);
	_hp_bar->activate();
	_box = jgl::w_box_component(this);
	_box.set_anchor(0);
	_box.set_border(4);
	_box.set_back(jgl::Color(133, 136, 140));
	_box.set_front(jgl::Color(170, 177, 189));
}
void Creature_slot::update()
{
	if (_creature != nullptr && *_creature != nullptr)
	{
		_hp_bar->set_ratio((*_creature)->HP().ratio());
		_hp_bar->set_max_value((*_creature)->HP().max);
		_hp_bar->set_print_value(true);
	}
	else
	{
		_hp_bar->set_ratio(0.0f);
		_hp_bar->set_print_value(false);
	}
}
void Creature_slot::select()
{
	_box.set_border(10);
	_box.set_back(jgl::Color(196, 195, 94));
}
void Creature_slot::unselect()
{
	_box.set_border(4);
	_box.set_back(jgl::Color(133, 136, 140));
}
void Creature_slot::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_anchor(p_anchor);
	_box.set_area(p_area);
	jgl::Vector2 sprite_size = _box.area().y - _box.border() * 2;
	float text_size = sprite_size.x / 3.0f;
	jgl::Vector2 pos = jgl::Vector2(sprite_size.x, text_size) + _box.border();
	_hp_bar->set_geometry(pos, jgl::Vector2(p_area.x - _box.border() * 2 - 10 - sprite_size.x, text_size));
}
void Creature_slot::render()
{
	_box.render(_viewport);
	if (_creature != nullptr && (*_creature) != nullptr)
	{
		Creature* tmp_creature = (*_creature)->species();
		jgl::Vector2 sprite_size = _box.area().y - _box.border() * 2;
		engine->faceset()->draw(tmp_creature->face(), _box.border(), sprite_size, 1.0f, _viewport);
		jgl::Vector2 pos = jgl::Vector2(sprite_size.x, 0.0f) + _box.border();
		jgl::draw_text(tmp_creature->name(), pos, sprite_size.x / 4.0f, 0, 1.0f, jgl::text_color::black, jgl::text_style::normal, _viewport);
	}
}

Creature_battle_slot::Creature_battle_slot(Creature_entity** p_creature, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_selected = false;
	_creature = p_creature;
	_hp_bar = new jgl::Progressive_bar(0.5f, jgl::Color(255, 0, 0), this);
	_hp_bar->activate();
	_box = jgl::w_box_component(this);
	_box.set_anchor(0);
	_box.set_border(4);
	_box.set_back(jgl::Color(133, 136, 140));
	_box.set_front(jgl::Color(170, 177, 189));
}
void Creature_battle_slot::update()
{
	if (_creature != nullptr && *_creature != nullptr)
	{
		_hp_bar->set_ratio((*_creature)->HP().ratio());
		_hp_bar->set_max_value((*_creature)->HP().max);
		_hp_bar->set_print_value(true);
	}
	else
	{
		_hp_bar->set_ratio(0.0f);
		_hp_bar->set_print_value(false);
	}
}
void Creature_battle_slot::select()
{
	_box.set_border(10);
	_box.set_back(jgl::Color(196, 195, 94));
}
void Creature_battle_slot::unselect()
{
	_box.set_border(4);
	_box.set_back(jgl::Color(133, 136, 140));
}
void Creature_battle_slot::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_anchor(p_anchor);
	_box.set_area(p_area);
	jgl::Vector2 sprite_size = _box.area().y - _box.border() * 2;
	float text_size = sprite_size.x / 3.0f;
	jgl::Vector2 pos = jgl::Vector2(sprite_size.x, text_size * 1.2f) + _box.border();
	_hp_bar->set_geometry(pos, jgl::Vector2(p_area.x - _box.border() * 2 - 10 - sprite_size.x, p_area.y - _box.border() * 2 - pos.y));
}
void Creature_battle_slot::render()
{
	_box.render(_viewport);
	if (_creature != nullptr && (*_creature) != nullptr)
	{
		Creature* tmp_creature = (*_creature)->species();
		jgl::Vector2 sprite_size = _box.area().y - _box.border() * 2;
		engine->faceset()->draw(tmp_creature->face(), _box.border(), sprite_size, 1.0f, _viewport);
		jgl::Vector2 pos = jgl::Vector2(sprite_size.x, 0.0f) + _box.border();
		jgl::draw_text(tmp_creature->name(), pos, sprite_size.x / 3.0f, 0, 1.0f, jgl::text_color::black, jgl::text_style::normal, _viewport);
	}
}