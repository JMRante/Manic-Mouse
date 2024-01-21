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

	assets.Load();

	game_state.mode = Gameplay;
	game_state.level_id = 0;

	LoadLevel(game_state.level_id);

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
			Vector2D mouse_direction = has_sampled_mouse_position ? next_mouse_position - input_state.mouse_position : Vector2D(1.0f, 0.0f);
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

			if (has_sampled_mouse_position) {
				input_state.last_mouse_position = input_state.mouse_position;
				input_state.mouse_position = next_mouse_position;
			} else {
				input_state.last_mouse_position = next_mouse_position;
				input_state.mouse_position = next_mouse_position;
			}

			has_sampled_mouse_position = true;
			break;
		}
	}
}

void Game::UpdateGameState(GameState& game_state, InputState& input_state, float delta_time_seconds) {
	Mouse& mouse = game_state.level.mouse;

	mouse.transform.SetPosition({
		input_state.mouse_position.x,
		input_state.mouse_position.y
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

void Game::LoadLevel(int level_id) {
	char test_level_map[920] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,
		1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,
		1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,
		1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	Tilemap& tilemap = game_state.level.tilemap;
	tilemap.active = true;
	tilemap.tilesheet_index = 0;

	for (int i = 0; i < 920; i++) {
		tilemap.tiles[i] = test_level_map[i];
	}

	int tilemap_vertex_count = 5 * 4 * 920;
	int tilemap_index_count = 6 * 920;

	float* vertices = new float[tilemap_vertex_count]();
	unsigned int* indices = new unsigned int[tilemap_index_count]();

	for (int i = 0; i < 920; i++) {
		int x = i % 40;
		int y = i / 40;

		Vector2D tilesheet_offset = tilemap.TilemapIndexToTilesheetOffset(i);

		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				int tile_quad_interval = i * 5 * 4;
				int tile_vertex_interval = (j + (k * 2)) * 5;
				int interval = tile_quad_interval + tile_vertex_interval;

				int tilesheet_width = assets.tile_sheets[tilemap.tilesheet_index].width;
				int tilesheet_height = assets.tile_sheets[tilemap.tilesheet_index].height;

				vertices[interval] = (x + j) * 32.0f;
				vertices[interval + 1] = (y + k) * -32.0f;
				vertices[interval + 2] = 0.0f;
				vertices[interval + 3] = (tilesheet_offset.x / tilesheet_width) + (j * (32.0f / tilesheet_width));
				vertices[interval + 4] = (tilesheet_offset.y / tilesheet_height) + ((1.0f - k) * (32.0f / tilesheet_height));
			}
		}

		int quad_index_interval = i * 6;
		int quad_vertex_interval = i * 4;

		indices[quad_index_interval] = quad_vertex_interval;
		indices[quad_index_interval + 1] = quad_vertex_interval + 2;
		indices[quad_index_interval + 2] = quad_vertex_interval + 1;
		indices[quad_index_interval + 3] = quad_vertex_interval + 1;
		indices[quad_index_interval + 4] = quad_vertex_interval + 2;
		indices[quad_index_interval + 5] = quad_vertex_interval + 3;
	}

	assets.LoadLevelMesh(vertices, tilemap_vertex_count * sizeof(float), indices, tilemap_index_count * sizeof(unsigned int));

	delete[] vertices;
	delete[] indices;
}