#include "erelia.h"

Dialogue_frame::Dialogue_frame(jgl::Widget* p_parent) : jgl::Frame(p_parent)
{

}

void Dialogue_frame::set_text(jgl::String p_text)
{
	_text = p_text;
	_tokens = _text.split(" ");
}

void Dialogue_frame::render()
{
	size_t text_size = static_cast<size_t>(_box.area().y) / 6;
	_box.render(_viewport);
	jgl::Vector2 pos = _box.border();
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		int len = jgl::calc_text_len(_tokens[i], text_size);
		if (pos.x + len >= _box.area().x)
		{
			pos.x = static_cast<float>(_box.border());
			pos.y += text_size;
		}
		jgl::draw_text(_tokens[i], pos, text_size, 0, 1.0f, jgl::text_color::black, jgl::text_style::normal, _viewport);
		pos.x += static_cast<float>(len + text_size);
	}
}