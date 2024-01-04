#include "assets.h"

#include <GL/glew.h>

#include "data_loader.h"

void Assets::Load() {
	sprite_shader_program = DataLoader::LoadShader("Assets/Shaders/sprite_shader.vert", "Assets/Shaders/sprite_shader.frag");
	sprite_sheet = DataLoader::LoadPNG("Assets/mm_tileset1.png");
}

void Assets::Unload() {
	glDeleteTextures(1, &sprite_sheet.id);

	glDeleteProgram(sprite_shader_program.id);
	glDeleteShader(sprite_shader_program.vertex_shader_id);
	glDeleteShader(sprite_shader_program.fragment_shader_id);
}