#pragma once
#ifndef MANICMOUSE_GAME_H_
#define MANICMOUSE_GAME_H_

#include <SDL.h>

#include "game_state.h"
#include "input_state.h"
#include "renderer.h"

class Game {
public:
	bool Start();
	void Run();
	void Close();
private:
	InputState input_state;
	GameState game_state;
	Renderer renderer;

	Assets assets;

	bool has_sampled_mouse_position = false;

	void UpdateInputState(InputState& input_state);
	void UpdateGameState(GameState& game_state, InputState& input_state, float delta_time_seconds);

	void LoadLevel(int level_id);
};

#endif