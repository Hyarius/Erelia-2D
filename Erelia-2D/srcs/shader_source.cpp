#include "Erelia.h"

GLuint programID;
GLuint delta_pos_uniform;

std::string custom_texture_shader_vert = {
	"#version 330 core\n"
	"layout(location = 0) in vec3 model_space;"
	"layout(location = 1) in vec2 vertexUV;"
	"uniform vec3 delta_pos;"
	"out vec2 UV;"
	"void main()"
	"{"
		"vec3 tmp = model_space - delta_pos;"// - vec3(-1, 1, 0);"
		"gl_Position = vec4(tmp, 1.0f);"
		"UV = vertexUV;"
	"}"
};

std::string custom_texture_shader_frag = {
	"#version 330 core\n"
	"in vec2 UV;"
	"out vec4 color;"
	"uniform sampler2D textureID;"
	"void main()"
	"{"
		"color = texture(textureID, UV).rgba;"
	"}"
};