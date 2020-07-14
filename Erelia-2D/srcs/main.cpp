#include "erelia-2D.h"

#define chunk_size 15
#define node_size 30

std::string custom_color_shader_vert = {
	"#version 330 core\n"
	"layout(location = 0) in vec3 model_space;"
	"layout(location = 1) in vec4 vertexColor;"
	"in vec3 delta_pos;"
	"out vec4 fragmentColor;"
	"void main()"
	"{"
		"gl_Position.xyz = model_space + delta_pos;"
		"fragmentColor = vertexColor;"
	"}"
};

std::string custom_color_shader_frag = {
	"#version 330 core\n"
	"in vec4 fragmentColor;"
	"out vec4 color;"
	"void main()"
	"{"
		"color = fragmentColor;"
	"}"
};

class Game_engine : public jgl::Widget
{
private:
	GLuint _program;
	GLuint _pos_delta;
	GLuint _buffer_array;
	GLuint _vertex_buffer;
	GLuint _color_buffer;

	jgl::Vector2 pos;
	jgl::Array<jgl::Pixel> _points;
	jgl::Array<jgl::Color> _colors;
public:
	Game_engine()
	{
		_program = g_application->add_custom_shader(custom_color_shader_vert, custom_color_shader_frag);
		_pos_delta = g_application->get_custom_uniform(_program, "delta_pos");

		glGenVertexArrays(1, &_buffer_array);
		glBindVertexArray(_buffer_array);

		glGenBuffers(1, &_vertex_buffer);
		glGenBuffers(1, &_color_buffer);

		pos = 0;
	}

	bool handle_keyboard()
	{
		return (false);
	}

	bool handle_mouse()
	{
		return (false);
	}

	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{

	}

	void render()
	{
		GLfloat* g_vertex_buffer_data = (float*)(_points.all());

		GLfloat* g_color_buffer_data = (float*)(_colors.all());

		glBindVertexArray(g_application->vertex_array());

		glBindBuffer(GL_ARRAY_BUFFER, g_application->vertex_buffer());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * _points.size() * 9, g_vertex_buffer_data);

		glBindBuffer(GL_ARRAY_BUFFER, g_application->color_buffer());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * _colors.size() * 12, g_color_buffer_data);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glUseProgram(g_application->program_color());

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, g_application->vertex_buffer());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, g_application->color_buffer());
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(3 * nb));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		pos += 1;
	}
};

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia 2D");
	app.set_max_fps(6000);
	jgl::set_font_path("ressources/font/karma suture.ttf"); 

	return (app.run());
}