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

	game_state.mode = Gameplay;
	game_state.level_id = 0;
	game_state.level.mouse.sprite.size = { 32.0f, 32.0f };
	game_state.level.mouse.sprite.offset = { 0.0f, 0.0f };
	game_state.level.mouse.transform = Transform();
	game_state.level.mouse.transform.SetScale({ 32.0f, 32.0f });

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

void Game::UpdateInputState(InputState& input_state) {
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_QUIT:
			input_state.window_closed = true;
			break;
		case SDL_MOUSEMOTION:
			input_state.mouse_position.x = (float)event.motion.x;
			input_state.mouse_position.y = (float)event.motion.y;
			break;
		}
	}
}

void Game::UpdateGameState(GameState& game_state, InputState& input_state) {
	game_state.level.mouse.transform.SetPosition({ 
		input_state.mouse_position.x,
		(renderer.window_height - input_state.mouse_position.y)
	});
}