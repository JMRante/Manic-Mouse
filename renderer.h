#pragma once
#ifndef MANICMOUSE_RENDERER_H_
#define MANICMOUSE_RENDERER_H_

#include <GL/glew.h>

struct ShaderProgram {
	GLuint vertex_shader_id = 0;
	GLuint fragment_shader_id = 0;
	GLuint shader_program_id = 0;
};

class Renderer
{
public:
	ShaderProgram sprite_shader_program;
	void Close();
};

#endif