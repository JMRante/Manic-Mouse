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

	game_state.mode = GameplayStart;
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

	input_state.mouse_left_pressed = false;

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_QUIT:
			input_state.window_closed = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				input_state.mouse_left_pressed = true;
			}
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
	Cheese& cheese = game_state.level.cheese;

	Key& red_key = game_state.level.red_key;
	Key& yellow_key = game_state.level.yellow_key;
	Key& blue_key = game_state.level.blue_key;

	Door& red_door = game_state.level.red_door;
	Door& yellow_door = game_state.level.yellow_door;
	Door& blue_door = game_state.level.blue_door;

	Transitions& transitions = game_state.transitions;

	if (!mouse.is_dead) {
		Vector2D mouse_last_position = mouse.transform.GetPosition();

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

		if (game_state.mode == GameplayStart) {
			if (IsPointCollidingWithCircle(mouse.transform.GetPosition(), game_state.level.start, 24.0f)) {
				game_state.mode = Gameplay;

				transitions.timer = 0.0f;
				transitions.radius_start = transitions.radius_goal;
				transitions.radius_goal = 2000.0f;
				transitions.transition_time = 0.3f;
			}
		} else if (game_state.mode == Gameplay) {
			if (IsPointCollidingWithCircle(mouse.transform.GetPosition(), cheese.transform.GetPosition(), 24.0f)) {
				game_state.mode = GameplayWon;

				transitions.active = true;
				transitions.timer = 0.0f;
				transitions.focus_point = cheese.transform.GetPosition();
				transitions.radius_start = transitions.radius_goal;
				transitions.radius_goal = 0.0f;
				transitions.transition_time = 0.3f;
			}

			Vector2D wall_collision_point;
			if (IsContinuousPointCollidingWithTileArray(mouse_last_position, mouse.transform.GetPosition(), game_state.level.tilemap.tiles, wall_collision_point)) {
				mouse.is_dead = true;
				mouse.sprite_index = 3;
				mouse.transform.SetPosition(wall_collision_point);
			}

			if (red_key.active && IsPointCollidingWithCircle(mouse.transform.GetPosition(), red_key.transform.GetPosition(), 24.0f)) {
				red_key.collected = true;
				red_key.transform.SetPosition({ 32.0f, renderer.window_height - 32.0f });
			}

			if (yellow_key.active && IsPointCollidingWithCircle(mouse.transform.GetPosition(), yellow_key.transform.GetPosition(), 24.0f)) {
				yellow_key.collected = true;
				yellow_key.transform.SetPosition({ 80.0f, renderer.window_height - 32.0f });
			}

			if (blue_key.active && IsPointCollidingWithCircle(mouse.transform.GetPosition(), blue_key.transform.GetPosition(), 24.0f)) {
				blue_key.collected = true;
				blue_key.transform.SetPosition({ 128.0f, renderer.window_height - 32.0f });
			}

			if (red_door.active && IsContinuousPointCollidingWithAABB(mouse_last_position, mouse.transform.GetPosition(), red_door.transform.GetPosition(), { 16.0f, 16.0f }, wall_collision_point)) {
				if (red_key.collected) {
					red_door.active = false;
				} else {
					mouse.is_dead = true;
					mouse.sprite_index = 3;
					mouse.transform.SetPosition(wall_collision_point);
				}
			}

			if (yellow_door.active && IsContinuousPointCollidingWithAABB(mouse_last_position, mouse.transform.GetPosition(), yellow_door.transform.GetPosition(), { 16.0f, 16.0f }, wall_collision_point)) {
				if (yellow_key.collected) {
					yellow_door.active = false;
				} else {
					mouse.is_dead = true;
					mouse.sprite_index = 3;
					mouse.transform.SetPosition(wall_collision_point);
				}
			}

			if (blue_door.active && IsContinuousPointCollidingWithAABB(mouse_last_position, mouse.transform.GetPosition(), blue_door.transform.GetPosition(), { 16.0f, 16.0f }, wall_collision_point)) {
				if (blue_key.collected) {
					blue_door.active = false;
				} else {
					mouse.is_dead = true;
					mouse.sprite_index = 3;
					mouse.transform.SetPosition(wall_collision_point);
				}
			}

			for (int i = 0; i < game_state.level.moving_block_count; i++) {
				MovingBlock& moving_block = game_state.level.moving_blocks[i];

				if (moving_block.behavior == Horizontal) {
					if ((moving_block.speed > 0 && IsPointCollidingWithTileArray(moving_block.transform.GetPosition() + Vector2D(16.0f, 0.0f), game_state.level.tilemap.tiles))
						|| (moving_block.speed < 0 && IsPointCollidingWithTileArray(moving_block.transform.GetPosition() + Vector2D(-16.0f, 0.0f), game_state.level.tilemap.tiles))) {
						moving_block.speed *= -1.0f;
					}

					moving_block.transform.SetPositionX(moving_block.transform.GetPosition().x + (moving_block.speed * delta_time_seconds));
				} else {
					if ((moving_block.speed > 0 && IsPointCollidingWithTileArray(moving_block.transform.GetPosition() + Vector2D(0.0f, 16.0f), game_state.level.tilemap.tiles))
						|| (moving_block.speed < 0 && IsPointCollidingWithTileArray(moving_block.transform.GetPosition() + Vector2D(0.0f, -16.0f), game_state.level.tilemap.tiles))) {
						moving_block.speed *= -1.0f;
					}

					moving_block.transform.SetPositionY(moving_block.transform.GetPosition().y + (moving_block.speed * delta_time_seconds));
				}

				if (IsContinuousPointCollidingWithAABB(mouse_last_position, mouse.transform.GetPosition(), moving_block.transform.GetPosition(), { 16.0f, 16.0f }, wall_collision_point)) {
					mouse.is_dead = true;
					mouse.sprite_index = 3;
					mouse.transform.SetPosition(wall_collision_point);
				}
			}
		}
	} else {
		if (game_state.mode == Gameplay) {
			game_state.mode = GameplayRestart;

			transitions.active = true;
			transitions.timer = 0.0f;
			transitions.focus_point = mouse.transform.GetPosition();
			transitions.radius_start = transitions.radius_goal;
			transitions.radius_goal = 0.0f;
			transitions.transition_time = 0.3f;
		}
	}

	if (transitions.active) {
		if (transitions.timer <= transitions.transition_time) {
			transitions.timer += delta_time_seconds;

			if (transitions.timer > transitions.transition_time) {
				transitions.timer = transitions.transition_time;

				if (game_state.mode == Gameplay) {
					transitions.active = false;
				} else if (game_state.mode == GameplayWon) {
					game_state.level_id = game_state.level_id < assets.levels.size() - 1 ? game_state.level_id + 1 : 0;
					LoadLevel(game_state.level_id);

					game_state.mode = GameplayStart;
				} else if (game_state.mode == GameplayRestart) {
					LoadLevel(game_state.level_id);

					game_state.mode = GameplayStart;
				}
			}
		}

		transitions.radius = Lerp(transitions.radius_start, transitions.radius_goal, transitions.timer / transitions.transition_time);
	}
}

void Game::LoadLevel(int level_id) {
	LevelState* level_to_load = assets.levels[level_id];

	game_state.level.start = level_to_load->start;

	game_state.level.mouse.is_dead = false;

	Cheese& cheese = game_state.level.cheese;
	cheese.transform.SetPosition(level_to_load->cheese.transform.GetPosition());

	Key& red_key = game_state.level.red_key;
	red_key.active = level_to_load->red_key.active;
	red_key.transform.SetPosition(level_to_load->red_key.transform.GetPosition());
	red_key.collected = false;

	Key& yellow_key = game_state.level.yellow_key;
	yellow_key.active = level_to_load->yellow_key.active;
	yellow_key.transform.SetPosition(level_to_load->yellow_key.transform.GetPosition());
	yellow_key.collected = false;

	Key& blue_key = game_state.level.blue_key;
	blue_key.active = level_to_load->blue_key.active;
	blue_key.transform.SetPosition(level_to_load->blue_key.transform.GetPosition());
	blue_key.collected = false;

	Door& red_door = game_state.level.red_door;
	red_door.active = level_to_load->red_door.active;
	red_door.transform.SetPosition(level_to_load->red_door.transform.GetPosition());

	Door& yellow_door = game_state.level.yellow_door;
	yellow_door.active = level_to_load->yellow_door.active;
	yellow_door.transform.SetPosition(level_to_load->yellow_door.transform.GetPosition());

	Door& blue_door = game_state.level.blue_door;
	blue_door.active = level_to_load->blue_door.active;
	blue_door.transform.SetPosition(level_to_load->blue_door.transform.GetPosition());

	game_state.level.moving_block_count = level_to_load->moving_block_count;

	for (int i = 0; i < 20; i++) {
		MovingBlock& moving_block = game_state.level.moving_blocks[i];
		moving_block.active = i < game_state.level.moving_block_count;
		moving_block.behavior = level_to_load->moving_blocks[i].behavior;
		moving_block.transform.SetPosition(level_to_load->moving_blocks[i].transform.GetPosition());
		moving_block.sprite.offset = moving_block.behavior == Vertical ? Vector2D(64.0f, 32.0f) : Vector2D(32.0f, 32.0f);
		moving_block.speed = 64.0f;
	}

	Tilemap& tilemap = game_state.level.tilemap;
	tilemap.active = true;
	tilemap.tilesheet_index = level_to_load->tilemap.tilesheet_index;

	for (int i = 0; i < 920; i++) {
		tilemap.tiles[i] = level_to_load->tilemap.tiles[i];
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

	Transitions& transitions = game_state.transitions;
	transitions.active = true;
	transitions.focus_point = level_to_load->start;
	transitions.timer = 0.0f;
	transitions.radius_start = 0.0f;
	transitions.radius_goal = 48.0f;
	transitions.transition_time = 0.3f;
}