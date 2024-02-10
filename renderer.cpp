#include "renderer.h"

#include <cmath>
#include <SDL.h>
#include <GL/glew.h>

#include "assets.h"
#include "game_state.h"

bool Renderer::Load(bool fullscreen) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	window_width = 1280;
	window_height = 720;

	int window_settings_flags = fullscreen ? SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_OPENGL;

	window = SDL_CreateWindow("Manic Mouse", 100, 100, window_width, window_height, window_settings_flags);
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

	tilemap_transform_matrix = Matrix4D();

	transition_transform_matrix = Matrix4D(
		{ 0.0f, 0.0f },
		0.0f,
		{ 2.0f * (float)window_width, 2.0f * (float)window_height }
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

	LevelState& level = game_state.level;

	// Tilemap
	if (level.tilemap.active) {
		glUseProgram(assets.tilemap_shader_program.id);
		glBindVertexArray(assets.level_mesh.vao_id);
		glBindTexture(GL_TEXTURE_2D, assets.tile_sheets[level.tilemap.tilesheet_index].id);

		GLint tilemap_transform_view_uniform_id = glGetUniformLocation(assets.tilemap_shader_program.id, "transform_view");
		Matrix4D tilemap_transform_view = tilemap_transform_matrix * view_matrix;
		glUniformMatrix4fv(tilemap_transform_view_uniform_id, 1, GL_TRUE, tilemap_transform_view.data);

		glDrawElements(GL_TRIANGLES, 6 * 920, GL_UNSIGNED_INT, 0);
	}

	// Sprites
	glUseProgram(assets.sprite_shader_program.id);
	glBindVertexArray(assets.quad_mesh.vao_id);
	glBindTexture(GL_TEXTURE_2D, assets.sprite_sheet.id);

	if (level.cheese.active) {
		RenderSprite(level.cheese.sprite, level.cheese.transform, assets);
	}

	if (level.red_key.active) {
		RenderSprite(level.red_key.sprite, level.red_key.transform, assets);
	}

	if (level.yellow_key.active) {
		RenderSprite(level.yellow_key.sprite, level.yellow_key.transform, assets);
	}

	if (level.blue_key.active) {
		RenderSprite(level.blue_key.sprite, level.blue_key.transform, assets);
	}

	if (level.red_door.active) {
		RenderSprite(level.red_door.sprite, level.red_door.transform, assets);
	}

	if (level.yellow_door.active) {
		RenderSprite(level.yellow_door.sprite, level.yellow_door.transform, assets);
	}

	if (level.blue_door.active) {
		RenderSprite(level.blue_door.sprite, level.blue_door.transform, assets);
	}

	for (int i = 0; i < level.moving_block_count; i++) {
		if (level.moving_blocks[i].active) {
			RenderSprite(level.moving_blocks[i].sprite, level.moving_blocks[i].transform, assets);
		}
	}

	// Transitions
	glUseProgram(assets.transition_shader_program.id);
	GLint focus_point_uniform_id = glGetUniformLocation(assets.transition_shader_program.id, "focus_point");
	glUniform2f(focus_point_uniform_id, game_state.transitions.focus_point.x, (float)window_height - game_state.transitions.focus_point.y);
	GLint timer_uniform_id = glGetUniformLocation(assets.transition_shader_program.id, "focus_radius");
	glUniform1f(timer_uniform_id, game_state.transitions.radius);
	GLint transition_transform_view_uniform_id = glGetUniformLocation(assets.transition_shader_program.id, "transform_view");
	Matrix4D transition_transform_view = transition_transform_matrix * view_matrix;
	glUniformMatrix4fv(transition_transform_view_uniform_id, 1, GL_TRUE, transition_transform_view.data);
	glDrawElements(GL_TRIANGLES, 6 * 920, GL_UNSIGNED_INT, 0);

	// Mouse
	glUseProgram(assets.sprite_shader_program.id);

	if (level.mouse.active) {
		RenderSprite(level.mouse.sprites[level.mouse.sprite_index], level.mouse.transform, assets);
	}

	// Timer
	if (!game_state.level.game_title.active) {
		for (int i = 0; i < 5; i++) {
			RenderSprite(game_state.level.timer.sprites[i], game_state.level.timer.transform[i], assets);
		}
	}

	// Title
	if (game_state.level.game_title.active) {
		RenderSprite(game_state.level.game_title.sprite, game_state.level.game_title.transform, assets);
	}

	SDL_GL_SwapWindow(window);
}

void Renderer::RenderSprite(Sprite& sprite, Transform& transform, Assets& assets) {
	GLint sprite_size_and_offset_uniform_id = glGetUniformLocation(assets.sprite_shader_program.id, "sprite_size_and_offset");
	glUniform4f(sprite_size_and_offset_uniform_id, sprite.size.x, sprite.size.y, sprite.offset.x, sprite.offset.y);
	GLint sprite_transform_view_uniform_id = glGetUniformLocation(assets.sprite_shader_program.id, "transform_view");
	Matrix4D sprite_transform_view = transform.GetTransformMatrix() * view_matrix;
	glUniformMatrix4fv(sprite_transform_view_uniform_id, 1, GL_TRUE, sprite_transform_view.data);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}