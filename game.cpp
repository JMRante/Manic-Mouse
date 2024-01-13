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

	if (!renderer.Load()) {
		return false;
	}

	// Setup universal data
	//InputState input_state;
	game_state.mode = Gameplay;
	game_state.level_id = 0;
	game_state.mouse.sprite.width = 32;
	game_state.mouse.sprite.height = 32;
	game_state.mouse.sprite.offset_x = 0;
	game_state.mouse.sprite.offset_y = 224;
	game_state.mouse.transform = Transform();
	game_state.mouse.transform.SetScaleX((float)game_state.mouse.sprite.width / (float)renderer.window_width);
	game_state.mouse.transform.SetScaleY((float)game_state.mouse.sprite.height / (float)renderer.window_height);

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
	renderer.Unload();

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

void Game::UpdateGameState(GameState& gameState, InputState& inputState) {

}