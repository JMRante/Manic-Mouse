#include "renderer.h"

#include <cmath>
#include <SDL.h>
#include <GL/glew.h>

#include "assets.h"
#include "game_state.h"

bool Renderer::Load() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	window_width = 1280;
	window_height = 720;

	window = SDL_CreateWindow("Manic Mouse", 100, 100, window_width, window_height, SDL_WINDOW_OPENGL);
	if (!window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	SDL_ShowCursor(SDL_FALSE);

	gl_context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(glew_error));
		return false;
	}

	view_matrix = Matrix4D(
		{ -1.0f, 1.0f }, 
		0.0f, 
		{ 2.0f / (float)window_width, 2.0f / (float)window_height }
	);

	return true;
}

void Renderer::Unload() {
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
}

void Renderer::Render(GameState& game_state, Assets& assets) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Tilemap
	glUseProgram(assets.tilemap_shader_program.id);
	glBindVertexArray(assets.level_mesh.vao_id);
	glBindTexture(GL_TEXTURE_2D, assets.tile_sheets[game_state.level.tilemap.tilesheet_index].id);

	GLint tilemap_transform_view_uniform_id = glGetUniformLocation(assets.tilemap_shader_program.id, "transform_view");
	Transform tile_transform = Transform();
	tile_transform.SetPosition({ 0.0f, 0.0f });
	Matrix4D tilemap_transform_view = tile_transform.GetTransformMatrix() * view_matrix;
	glUniformMatrix4fv(tilemap_transform_view_uniform_id, 1, GL_TRUE, tilemap_transform_view.data);

	glDrawElements(GL_TRIANGLES, 6 * 920, GL_UNSIGNED_INT, 0);

	// Sprites
	glUseProgram(assets.sprite_shader_program.id);
	glBindVertexArray(assets.quad_mesh.vao_id);
	glBindTexture(GL_TEXTURE_2D, assets.sprite_sheet.id);

	Sprite& sprite = game_state.level.mouse.sprites[game_state.level.mouse.sprite_index];
	GLint sprite_size_and_offset_uniform_id = glGetUniformLocation(assets.sprite_shader_program.id, "sprite_size_and_offset");
	glUniform4f(sprite_size_and_offset_uniform_id, sprite.size.x, sprite.size.y, sprite.offset.x, sprite.offset.y);
	GLint sprite_transform_view_uniform_id = glGetUniformLocation(assets.sprite_shader_program.id, "transform_view");
	Matrix4D sprite_transform_view = game_state.level.mouse.transform.GetTransformMatrix() * view_matrix;
	glUniformMatrix4fv(sprite_transform_view_uniform_id, 1, GL_TRUE, sprite_transform_view.data);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(window);
}