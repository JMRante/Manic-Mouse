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

	gl_context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(glew_error));
		return false;
	}

	return true;
}

void Renderer::Unload() {
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
}

void Renderer::Render(const GameState& game_state, const Assets& assets) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(assets.sprite_shader_program.id);
	glBindTexture(GL_TEXTURE_2D, assets.sprite_sheet.id);
	glBindVertexArray(assets.quad_mesh.vao_id);

	Sprite sprite = game_state.test_sprite;
	GLint sprite_size_and_offset_uniform_id = glGetUniformLocation(assets.sprite_shader_program.id, "sprite_size_and_offset");
	glUniform4f(sprite_size_and_offset_uniform_id, sprite.width, sprite.height, sprite.offset_x, sprite.offset_y);
	GLint sprite_sheet_size_uniform_id = glGetUniformLocation(assets.sprite_shader_program.id, "sprite_sheet_size");
	glUniform2f(sprite_sheet_size_uniform_id, assets.sprite_sheet.width, assets.sprite_sheet.height);
	float deg_to_rad = 0.01745329f;
	float rotation_radians = sprite.rotation_degrees * deg_to_rad;
	float transform[] = {
		cos(rotation_radians) * (sprite.width / (float)window_width), -sin(rotation_radians), 0.0f, 0.0f,
		sin(rotation_radians), cos(rotation_radians) * (sprite.height / (float)window_height), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		sprite.x, sprite.y, 0.0f, 1.0f,
	};
	GLint transform_uniform_id = glGetUniformLocation(assets.sprite_shader_program.id, "transform");
	glUniformMatrix4fv(transform_uniform_id, 1, GL_TRUE, reinterpret_cast<const float*>(&transform[0]));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(window);
}