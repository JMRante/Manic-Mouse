#include "renderer.h"

#include <GL/glew.h>

void Renderer::Close() {
	glDeleteProgram(sprite_shader_program.shader_program_id);
	glDeleteShader(sprite_shader_program.vertex_shader_id);
	glDeleteShader(sprite_shader_program.fragment_shader_id);
}

#include <GL/glew.h>