#include "erelia.h"

Item::Item(Item_type p_item_type, jgl::Vector2 p_sprite)
{
	_item_type = p_item_type;
	_sprite = p_sprite;
}

void Item::draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, const jgl::Viewport* p_viewport)
{
	engine->tileset()->draw(_sprite, p_anchor, p_area, 1.0f, p_viewport);
}

void Prefab_item::draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, const jgl::Viewport* p_viewport)
{
	engine->tileset()->draw(_sprite, p_anchor, p_area, 1.0f, p_viewport);
	jgl::draw_centred_text(_prefab->name(), p_anchor + p_area / 2, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, nullptr);
}

void NPC_item::use(jgl::Data param)
{
	jgl::Vector2* start = param.acces<jgl::Vector2*>(0);
	NPC* new_npc = _npc->clone();
	new_npc->check_point().clear();
	new_npc->set_starting_pos(*start);
	new_npc->place(*start);
	new_npc->add_check_point(new_npc->pos());
	engine->board()->add_npc(new_npc);
}

void NPC_item::draw(jgl::Vector2 p_anchor, jgl::Vector2 p_area, const jgl::Viewport* p_viewport)
{
	engine->charset()->draw(jgl::Vector2(1.0f, _npc->sprite().y), p_anchor, p_area, 1.0f, p_viewport);
	jgl::String text = "";
	for (size_t i = 0; i < 4 && i < _npc->name().size(); i++)
		text += _npc->name()[i];
	jgl::draw_centred_text(text, p_anchor + p_area / 2, 16, 1, 1.0f, jgl::text_color::white, jgl::text_style::normal, nullptr);
}