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
	jgl::draw_centred_text(_prefab->name(), p_anchor + p_area / 2, 16, 1, jgl::text_color::white, jgl::text_style::normal, nullptr);
}