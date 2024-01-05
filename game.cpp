#include "game.h"

#include <SDL.h>
#include <GL/glew.h>

#include "assets.h"
#include "game_state.h"
#include "input_state.h"
#include "renderer.h"

bool Game::Start() {
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	window = SDL_CreateWindow("Manic Mouse", 100, 100, 1280, 720, SDL_WINDOW_OPENGL);
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

	// Setup universal data
	//InputState input_state;
	//GameState game_state;

	assets.Load();

	return true;
}

void Game::Run() {
	bool is_running = true;
	Uint32 tick_count = 0;

	while (is_running) {
		float delta_time_seconds = (SDL_GetTicks() - tick_count) / 1000.0f;
		if (delta_time_seconds > 0.05f) {
			delta_time_seconds = 0.05f;
		}

		tick_count = SDL_GetTicks();

		UpdateInputState(input_state);

		if (input_state.window_closed) {
			is_running = false;
			break;
		}

		UpdateGameState(game_state, input_state);

		renderer.Render(game_state, assets);
	}
}

void Game::Close() {
	assets.Unload();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::UpdateInputState(InputState& inputState) {
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_QUIT:
			inputState.window_closed = true;
			break;
		}
	}
}

void Game::UpdateGameState(GameState& gameState, const InputState& inputState) {

}