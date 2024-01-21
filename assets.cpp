#include "assets.h"

#include <GL/glew.h>

#include "data_loader.h"

void Assets::Load() {
	sprite_shader_program = DataLoader::LoadShader("Assets/Shaders/sprite_shader.vert", "Assets/Shaders/sprite_shader.frag");
	tilemap_shader_program = DataLoader::LoadShader("Assets/Shaders/tilemap_shader.vert", "Assets/Shaders/sprite_shader.frag");
	sprite_sheet = DataLoader::LoadPNG("Assets/mm_sprites.png");

	tile_sheets[0] = DataLoader::LoadPNG("Assets/mm_tileset1.png");
	tile_sheets[1] = DataLoader::LoadPNG("Assets/mm_tileset2.png");
	tile_sheets[2] = DataLoader::LoadPNG("Assets/mm_tileset3.png");
	tile_sheets[3] = DataLoader::LoadPNG("Assets/mm_tileset4.png");
	tile_sheets[4] = DataLoader::LoadPNG("Assets/mm_tileset5.png");

	DataLoader::LoadLevels("Assets/LevelData/level_data.lvls", levels);

	LoadQuadMesh();
}

void Assets::Unload() {
	glDeleteTextures(1, &sprite_sheet.id);

	glDeleteProgram(sprite_shader_program.id);
	glDeleteShader(sprite_shader_program.vertex_shader_id);
	glDeleteShader(sprite_shader_program.fragment_shader_id);

	glDeleteProgram(tilemap_shader_program.id);
	glDeleteShader(tilemap_shader_program.vertex_shader_id);
	glDeleteShader(tilemap_shader_program.fragment_shader_id);

	glDeleteVertexArrays(1, &quad_mesh.vao_id);
	glDeleteBuffers(1, &quad_mesh.vbo_id);
	glDeleteBuffers(1, &quad_mesh.ebo_id);

	glDeleteVertexArrays(1, &level_mesh.vao_id);
	glDeleteBuffers(1, &level_mesh.vbo_id);
	glDeleteBuffers(1, &level_mesh.ebo_id);

	for (int i = 0; i < levels.size(); i++) {
		delete levels[i];
	}
}

void Assets::LoadLevelMesh(float* vertices, unsigned int vertex_length, unsigned int* indices, unsigned int index_length) {
	LoadMesh(level_mesh, vertices, vertex_length, indices, index_length);
}

void Assets::LoadMesh(Mesh& mesh, float* vertices, unsigned int vertex_length, unsigned int* indices, unsigned int index_length) {
	glGenVertexArrays(1, &mesh.vao_id);
	glBindVertexArray(mesh.vao_id);

	glGenBuffers(1, &mesh.vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, vertex_length, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &mesh.ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_length, indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(sizeof(float) * 3));
}

void Assets::LoadQuadMesh() {
	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	LoadMesh(quad_mesh, vertices, sizeof(vertices), indices, sizeof(indices));
}