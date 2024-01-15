#include "game.h"

#include <cmath>
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

	Mouse& mouse = game_state.level.mouse;
	mouse.sprites[0].size = {32.0f, 32.0f};
	mouse.sprites[0].offset = { 0.0f, 0.0f };
	mouse.sprites[1].size = { 32.0f, 32.0f };
	mouse.sprites[1].offset = { 32.0f, 0.0f };
	mouse.sprites[2].size = { 32.0f, 32.0f };
	mouse.sprites[2].offset = { 64.0f, 0.0f };
	mouse.sprite_index = 0;
	mouse.sprite_speed = 0.15f;
	mouse.transform = Transform();
	mouse.transform.SetScale({ 32.0f, 32.0f });

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

		UpdateGameState(game_state, input_state, delta_time_seconds);

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

	input_state.mouse_speed = 0.0f;

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_QUIT:
			input_state.window_closed = true;
			break;
		case SDL_MOUSEMOTION:
			Vector2D next_mouse_position = { (float)event.motion.x, (float)event.motion.y };
			Vector2D mouse_direction = next_mouse_position - input_state.mouse_position;
			input_state.mouse_direction_smoothed = mouse_direction;

			for (int i = 0; i < 10; i++) {
				if (i < 9) {
					input_state.mouse_direction_history[i] = input_state.mouse_direction_history[i + 1];
					input_state.mouse_direction_smoothed += input_state.mouse_direction_history[i];
				} else {
					input_state.mouse_direction_history[i] = mouse_direction;
				}
			}

			input_state.mouse_direction_smoothed.x /= 10;
			input_state.mouse_direction_smoothed.y /= 10;

			input_state.mouse_speed = mouse_direction.Length();
			input_state.mouse_position = next_mouse_position;
			break;
		}
	}
}

void Game::UpdateGameState(GameState& game_state, InputState& input_state, float delta_time_seconds) {
	Mouse& mouse = game_state.level.mouse;

	mouse.transform.SetPosition({
		input_state.mouse_position.x,
		renderer.window_height - input_state.mouse_position.y
	});

	mouse.sprite_timer += input_state.mouse_speed * delta_time_seconds;
	
	if (mouse.sprite_timer >= mouse.sprite_speed) {
		mouse.sprite_timer -= mouse.sprite_speed;
		mouse.sprite_index++;

		if (mouse.sprite_index >= 3) {
			mouse.sprite_index = 0;
		}
	}

	float angle = Vector2D::Angle(Vector2D::up * -1.0f, input_state.mouse_direction_smoothed);
	angle = copysign(angle, input_state.mouse_direction_smoothed.x);
	mouse.transform.SetRotationInRadians(angle);
}