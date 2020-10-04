#include "erelia.h"

Battle_face_renderer::Battle_face_renderer(Creature_entity** p_ally_entity, Creature_entity** p_enemy_entity, jgl::Widget* parent) : jgl::Widget(parent)
{
	_ally_entity = p_ally_entity;
	_enemy_entity = p_enemy_entity;
	_old_ally_entity = nullptr;
	_old_enemy_entity = nullptr;

	_frame = new jgl::Frame(this);
	_frame->activate();

	_enemy_name = new jgl::Text_label("", _frame);
	_enemy_name->set_align(jgl::alignment::left);
	_enemy_name->activate();
	_enemy_name->set_back(jgl::Color(130, 130, 130));
	_enemy_name->set_front(jgl::Color(220, 220, 220));

	_enemy_face = new jgl::Image_label(nullptr, _frame);
	_enemy_face->set_back(jgl::Color(130, 130, 130));
	_enemy_face->set_front(jgl::Color(220, 220, 220));
	_enemy_face->activate();

	_enemy_hp_bar = new jgl::Progressive_bar(0, jgl::Color(227, 14, 7), _frame);
	_enemy_hp_bar->set_back(jgl::Color(130, 130, 130));
	_enemy_hp_bar->set_front(jgl::Color(220, 220, 220));
	_enemy_hp_bar->activate();


	_ally_name = new jgl::Text_label("", _frame);
	_ally_name->set_align(jgl::alignment::left);
	_ally_name->activate();
	_ally_name->set_back(jgl::Color(130, 130, 130));
	_ally_name->set_front(jgl::Color(220, 220, 220));

	_ally_face = new jgl::Image_label(nullptr, _frame);
	_ally_face->set_back(jgl::Color(130, 130, 130));
	_ally_face->set_front(jgl::Color(220, 220, 220));
	_ally_face->activate();

	_ally_hp_bar = new jgl::Progressive_bar(0, jgl::Color(227, 14, 7), _frame);
	_ally_hp_bar->set_back(jgl::Color(130, 130, 130));
	_ally_hp_bar->set_front(jgl::Color(220, 220, 220));
	_ally_hp_bar->activate();
}

void Battle_face_renderer::update()
{
	if (_ally_entity != nullptr && _old_ally_entity != *_ally_entity)
	{
		_old_ally_entity = *_ally_entity;
		if (_old_ally_entity != nullptr)
		{
			_ally_name->set_text(_old_ally_entity->name());
			_ally_name->label().calc_text_size(_ally_name->area() - _ally_name->box().border() * 2);
			_ally_face->set_sprite(_old_ally_entity->species()->face());
			_ally_face->set_image(engine->back_faceset());
			_ally_hp_bar->set_ratio(_old_ally_entity->HP().actual / _old_ally_entity->HP().max);
		}
		else
		{
			_ally_name->set_text("");
			_ally_face->set_sprite(-1);
			_ally_face->set_image(static_cast<jgl::Sprite_sheet*>(nullptr));
			_ally_hp_bar->set_ratio(0);
		}
	}
	if (_enemy_entity != nullptr && _old_enemy_entity != *_enemy_entity)
	{
		_old_enemy_entity = *_enemy_entity;
		if (_enemy_entity != nullptr)
		{
			_old_enemy_entity = *_enemy_entity;
			_enemy_name->set_text(_old_enemy_entity->name());
			_enemy_name->label().calc_text_size(_enemy_name->area() - _enemy_name->box().border() * 2);
			_enemy_face->set_sprite(_old_enemy_entity->species()->face());
			_enemy_face->set_image(engine->faceset());
			_enemy_hp_bar->set_ratio(_old_enemy_entity->HP().actual / _old_enemy_entity->HP().max);
		}
		else
		{
			_old_enemy_entity = nullptr;
			_enemy_name->set_text("");
			_enemy_face->set_sprite(-1);
			_enemy_face->set_image(static_cast<jgl::Sprite_sheet*>(nullptr));
			_enemy_hp_bar->set_ratio(0);
		}
	}
}

void Battle_face_renderer::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	float face_size = 100; 
	_frame->set_geometry(0, p_area);
	jgl::Vector2 pos = 10;
	jgl::Vector2 size = jgl::Vector2(p_area.x - 25 - face_size, face_size / 3);
	_enemy_name->set_geometry(pos, size);
	pos.y += face_size / 3 + 5;
	_enemy_hp_bar->set_geometry(pos, size);
	_enemy_face->set_geometry(jgl::Vector2(p_area.x - 10 - face_size, 10.0f), face_size);

	_ally_face->set_geometry(jgl::Vector2(0.0f, face_size + 5) + 10, face_size);
	pos = jgl::Vector2(face_size, face_size + face_size - face_size / 3) + 15;
	_ally_name->set_geometry(pos, size);
	pos.y -= face_size / 3 + 5;
	_ally_hp_bar->set_geometry(pos, size);
}

void Battle_face_renderer::render()
{

}