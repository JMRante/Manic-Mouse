#pragma once
#ifndef MANICMOUSE_ASSETS_H_
#define MANICMOUSE_ASSETS_H_

#include <SDL_mixer.h>
#include <GL/glew.h>
#include <vector>

#include "game_state.h"

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
	ShaderProgram tilemap_shader_program;
	ShaderProgram transition_shader_program;
	Texture sprite_sheet;
	Texture tile_sheets[5];
	Mesh quad_mesh;
	Mesh level_mesh;

	Mix_Chunk* key_collect_sound;
	Mix_Chunk* open_door_sound;
	Mix_Chunk* mouse_die_sound;
	Mix_Chunk* eat_cheese_sound;

	Mix_Music* background_music;

	std::vector<LevelState*> levels;

	void Load();
	void Unload();

	void LoadLevelMesh(float* vertices, unsigned int vertex_length, unsigned int* indices, unsigned int index_length);
private:
	void LoadMesh(Mesh& mesh, float* vertices, unsigned int vertex_length, unsigned int* indices, unsigned int index_length);
	void LoadQuadMesh();
};

#endif