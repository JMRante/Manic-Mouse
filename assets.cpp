#include "assets.h"

#include <GL/glew.h>

#include "data_loader.h"

void Assets::Load() {
	sprite_shader_program = DataLoader::LoadShader("Assets/Shaders/sprite_shader.vert", "Assets/Shaders/sprite_shader.frag");
	sprite_sheet = DataLoader::LoadPNG("Assets/mm_sprites.png");

	LoadQuadMesh();
}

void Assets::Unload() {
	glDeleteTextures(1, &sprite_sheet.id);

	glDeleteProgram(sprite_shader_program.id);
	glDeleteShader(sprite_shader_program.vertex_shader_id);
	glDeleteShader(sprite_shader_program.fragment_shader_id);

	glDeleteVertexArrays(1, &quad_mesh.vao_id);
	glDeleteBuffers(1, &quad_mesh.vbo_id);
	glDeleteBuffers(1, &quad_mesh.ebo_id);
}

void Assets::LoadQuadMesh() {
	float* vertices = new float[] {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	unsigned int* indices = new unsigned int[] {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &quad_mesh.vao_id);
	glGenBuffers(1, &quad_mesh.vbo_id);
	glGenBuffers(1, &quad_mesh.ebo_id);

	glBindVertexArray(quad_mesh.vao_id);

	glBindBuffer(GL_ARRAY_BUFFER, quad_mesh.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_mesh.ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	delete[] vertices;
	delete[] indices;
}