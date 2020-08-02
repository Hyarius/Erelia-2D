#include "erelia.h"

jgl::Vector2 tile_to_screen(jgl::Vector2 player_pos, jgl::Vector2 tile_pos)
{
	jgl::Vector2 result = (tile_pos - player_pos) * node_size + g_application->size() / 2;
	return (result);
}
jgl::Vector2 screen_to_tile(jgl::Vector2 player_pos, jgl::Vector2 coord)
{
	jgl::Vector2 pos = coord - g_application->size() / 2;
	jgl::Vector2 rel = pos / node_size;

	return ((player_pos + rel).floor());
}

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia 2D", 0.8f);
	//app.set_max_fps(60);
	jgl::set_font_path("ressources/font/karma suture.ttf");

	programID = g_application->add_custom_shader(custom_texture_shader_vert, custom_texture_shader_frag);
	delta_pos_uniform = g_application->get_custom_uniform(programID, "delta_pos");

	Game_engine* engine = new Game_engine();
	engine->set_geometry(0, app.size());
	engine->activate();

	return (app.run());
}