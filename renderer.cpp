#include "renderer.h"

#include <cmath>
#include <SDL.h>
#include <GL/glew.h>
#include <string>

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
	if (!game_state.opening) {
		std::string timer_string = std::to_string(level.time_limit);

		if (level.time_limit < 10.0f) {
			timer_string.insert(0, 1, '0');
		}

		for (int i = 0; i < 5; i++) {
			char ch = timer_string[i];

			Sprite char_sprite;
			Transform char_sprite_transform = Transform();
			float ch_x = ((float)window_width / 2.0f) - 64.0f;

			if (i == 1) ch_x += 28.0f;
			if (i == 2) ch_x += 52.0f;
			if (i == 3) ch_x += 76.0f;
			if (i == 4) ch_x += 104.0f;

			char_sprite_transform.SetPosition({ ch_x, 32.0f });

			switch (ch) {
			case '0':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 96.0f, 32.0f };
				break;
			case '1':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 120.0f, 32.0f };
				break;
			case '2':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 96.0f, 64.0f };
				break;
			case '3':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 120.0f, 64.0f };
				break;
			case '4':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 96.0f, 96.0f };
				break;
			case '5':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 120.0f, 96.0f };
				break;
			case '6':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 144.0f, 96.0f };
				break;
			case '7':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 168.0f, 96.0f };
				break;
			case '8':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 192.0f, 96.0f };
				break;
			case '9':
				char_sprite.size = { 24.0f, 32.0f };
				char_sprite.offset = { 216.0f, 96.0f };
				break;
			case '.':
				char_sprite.size = { 16.0f, 32.0f };
				char_sprite.offset = { 240.0f, 96.0f };
				break;
			}

			char_sprite_transform.SetScale(char_sprite.size);

			RenderSprite(char_sprite, char_sprite_transform, assets);
		}
	}

	// Title
	if (game_state.opening) {
		Sprite menu_sprite;
		menu_sprite.size = { 256.0f, 128.0f };
		menu_sprite.offset = { 0.0f, 128.0f };
		
		Transform menu_transform;
		menu_transform.SetPosition({ (float)window_width / 2.0f, ((float)window_height / 2.0f) - 16.0f });
		menu_transform.SetScale(menu_sprite.size);

		RenderSprite(menu_sprite, menu_transform, assets);
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