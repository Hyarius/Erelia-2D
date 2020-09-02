#include "erelia.h"

Game_engine* engine;
int node_size = 32;

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia 2D", 0.8f);
	//node_size = static_cast<int>(min(app.size().x / 19, app.size().y / 10));
	app.set_max_fps(12000);
	jgl::set_font_path("ressources/font/karma suture.ttf");

	programID = g_application->add_custom_shader(custom_texture_shader_vert, custom_texture_shader_frag);
	delta_pos_uniform = g_application->get_custom_uniform(programID, "delta_pos");

	new Game_engine();
	engine->set_geometry(0, app.size());
	engine->activate();

	return (app.run());
}