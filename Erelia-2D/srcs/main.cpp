#include "erelia.h"

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia 2D", jgl::Vector2(640, 420));
	//app.set_max_fps(60);
	jgl::set_font_path("ressources/font/karma suture.ttf");

	programID = g_application->add_custom_shader(custom_texture_shader_vert, custom_texture_shader_frag);
	delta_pos_uniform = g_application->get_custom_uniform(programID, "delta_pos");

	Game_engine* engine = new Game_engine();
	engine->set_geometry(0, app.size());
	engine->activate();

	return (app.run());
}