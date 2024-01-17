#pragma once
#ifndef MANICMOUSE_ASSETS_H_
#define MANICMOUSE_ASSETS_H_

#include <GL/glew.h>

struct ShaderProgram {
	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint id;
};

struct Texture {
	int width;
	int height;
	GLuint id;
};

struct Mesh {
	GLuint vao_id;
	GLuint vbo_id;
	GLuint ebo_id;
};

class Assets
{
public:
	ShaderProgram sprite_shader_program;
	Texture sprite_sheet;
	Texture tile_sheets[5];
	Mesh quad_mesh;

	void Load();
	void Unload();
private:
	void LoadQuadMesh();
};

#endif