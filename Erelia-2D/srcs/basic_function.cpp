#include "Erelia.h"

jgl::Vector2 tile_to_screen(jgl::Vector2 tile_pos)
{
	jgl::Vector2 result = ((tile_pos - engine->player()->pos() - 0.5f) * node_size + g_application->size() / 2);
	return (result);
}
jgl::Vector2 screen_to_tile(jgl::Vector2 coord)
{
	jgl::Vector2 pos = (coord - g_application->size() / 2);
	jgl::Vector2 rel = pos / node_size;

	return ((engine->player()->pos() + rel + 0.5f).floor());
}